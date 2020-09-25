//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:         BSD License
//                   Kratos default license: kratos/license.txt
//
//  Main authors:
//

#if !defined(KRATOS_RESIDUAL_BASED_ADJOINT_BOSSAK_SCHEME_H_INCLUDED)
#define KRATOS_RESIDUAL_BASED_ADJOINT_BOSSAK_SCHEME_H_INCLUDED

// System includes
#include <vector>
#include <string>
#include <unordered_set>
#include <functional>

// External includes

// Project includes
#include "includes/define.h"
#include "includes/checks.h"
#include "includes/kratos_parameters.h"
#include "solving_strategies/schemes/scheme.h"
#include "response_functions/adjoint_response_function.h"
#include "utilities/variable_utils.h"
#include "utilities/indirect_scalar.h"
#include "utilities/adjoint_extensions.h"
#include "utilities/parallel_utilities.h"

namespace Kratos
{
///@name Kratos Classes
///@{

/// A scheme for dynamic adjoint equations, using Bossak time integration.
/**
 * It can be used for either first- or second-order time derivatives. Elements
 * and conditions must provide a specialization of AdjointExtensions via their
 * data value container, which allows the scheme to operate independently of
 * the variable arrangements in the element or condition.
 */
template <class TSparseSpace, class TDenseSpace>
class ResidualBasedAdjointBossakScheme : public Scheme<TSparseSpace, TDenseSpace>
{
public:
    ///@name Type Definitions
    ///@{

    KRATOS_CLASS_POINTER_DEFINITION(ResidualBasedAdjointBossakScheme);

    typedef Scheme<TSparseSpace, TDenseSpace> BaseType;

    typedef typename BaseType::TSystemMatrixType SystemMatrixType;

    typedef typename BaseType::TSystemVectorType SystemVectorType;

    typedef typename BaseType::LocalSystemVectorType LocalSystemVectorType;

    typedef typename BaseType::LocalSystemMatrixType LocalSystemMatrixType;

    typedef typename BaseType::DofsArrayType DofsArrayType;

    ///@}
    ///@name Life Cycle
    ///@{

    /// Constructor.
    ResidualBasedAdjointBossakScheme(
        Parameters Settings,
        AdjointResponseFunction::Pointer pResponseFunction
        ) : mpResponseFunction(pResponseFunction)
    {
        Parameters default_parameters(R"({
            "name"         : "adjoint_bossak",
            "scheme_type"  : "bossak",
            "alpha_bossak" : -0.3
        })");
        Settings.ValidateAndAssignDefaults(default_parameters);
        mBossak.Alpha = Settings["alpha_bossak"].GetDouble();
    }

    /// Destructor.
    ~ResidualBasedAdjointBossakScheme() override
    {
    }

    ///@}
    ///@name Operators
    ///@{

    ///@}
    ///@name Operations
    ///@{

    void Initialize(ModelPart& rModelPart) override
    {
        KRATOS_TRY;

        BaseType::Initialize(rModelPart);

        // Allocate auxiliary memory.
        int num_threads = OpenMPUtils::GetNumThreads();
        mLeftHandSide.resize(num_threads);
        mResponseGradient.resize(num_threads);
        mFirstDerivsLHS.resize(num_threads);
        mFirstDerivsResponseGradient.resize(num_threads);
        mSecondDerivsLHS.resize(num_threads);
        mSecondDerivsResponseGradient.resize(num_threads);
        mAdjointValuesVector.resize(num_threads);
        mAdjointIndirectVector2.resize(num_threads);
        mAdjointIndirectVector3.resize(num_threads);
        mAuxAdjointIndirectVector1.resize(num_threads);

        VariableUtils().SetNonHistoricalVariableToZero(NUMBER_OF_NEIGHBOUR_ELEMENTS, rModelPart.Nodes());

        rModelPart.GetProcessInfo()[BOSSAK_ALPHA] = mBossak.Alpha;

        KRATOS_CATCH("");
    }

    void InitializeSolutionStep(ModelPart& rModelPart,
                                SystemMatrixType& rA,
                                SystemVectorType& rDx,
                                SystemVectorType& rb) override
    {
        KRATOS_TRY;

        BaseType::InitializeSolutionStep(rModelPart, rA, rDx, rb);

        const auto& r_current_process_info = rModelPart.GetProcessInfo();
        mBossak = CalculateBossakConstants(mBossak.Alpha, GetTimeStep(r_current_process_info));

        this->CalculateNodeNeighbourCount(rModelPart);

        KRATOS_CATCH("");
    }

    void FinalizeSolutionStep(ModelPart& rModelPart,
                              SystemMatrixType& rA,
                              SystemVectorType& rDx,
                              SystemVectorType& rb) override
    {
        KRATOS_TRY;

        BaseType::FinalizeSolutionStep(rModelPart, rA, rDx, rb);
        this->UpdateAuxiliaryVariable(rModelPart);

        KRATOS_CATCH("");
    }

    void Update(ModelPart& rModelPart,
                DofsArrayType& rDofSet,
                SystemMatrixType& rA,
                SystemVectorType& rDx,
                SystemVectorType& rb) override
    {
        KRATOS_TRY;

        // Update degrees of freedom: adjoint variables associated to the
        // residual of the physical problem.
        this->mpDofUpdater->UpdateDofs(rDofSet, rDx);
        // Update adjoint variables associated to time integration.
        this->UpdateTimeSchemeAdjoints(rModelPart);

        KRATOS_CATCH("");
    }

    void CalculateSystemContributions(Element& rCurrentElement,
                                      LocalSystemMatrixType& rLHS_Contribution,
                                      LocalSystemVectorType& rRHS_Contribution,
                                      Element::EquationIdVectorType& rEquationId,
                                      const ProcessInfo& rCurrentProcessInfo) override
    {
        KRATOS_TRY;

        const auto k = OpenMPUtils::ThisThread();
        rCurrentElement.GetValuesVector(mAdjointValuesVector[k]);
        const auto local_size = mAdjointValuesVector[k].size();
        if (rRHS_Contribution.size() != local_size)
        {
            rRHS_Contribution.resize(local_size, false);
        }
        if (rLHS_Contribution.size1() != local_size || rLHS_Contribution.size2() != local_size)
        {
            rLHS_Contribution.resize(local_size, local_size, false);
        }
        this->CheckAndResizeThreadStorage(local_size);

        this->CalculateGradientContributions(rCurrentElement, rLHS_Contribution,
                                             rRHS_Contribution, rCurrentProcessInfo);

        this->CalculateFirstDerivativeContributions(
            rCurrentElement, rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);

        this->CalculateSecondDerivativeContributions(
            rCurrentElement, rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);

        this->CalculatePreviousTimeStepContributions(
            rCurrentElement, rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);

        this->CalculateResidualLocalContributions(
            rCurrentElement, rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);

        rCurrentElement.EquationIdVector(rEquationId, rCurrentProcessInfo);

        KRATOS_CATCH("");
    }

    void CalculateLHSContribution(Element& rCurrentElement,
                                  LocalSystemMatrixType& rLHS_Contribution,
                                  Element::EquationIdVectorType& rEquationId,
                                  const ProcessInfo& rCurrentProcessInfo) override
    {
        KRATOS_TRY;
        LocalSystemVectorType RHS_Contribution;
        CalculateSystemContributions(rCurrentElement, rLHS_Contribution, RHS_Contribution,
                                     rEquationId, rCurrentProcessInfo);
        KRATOS_CATCH("");
    }

    void CalculateSystemContributions(Condition& rCurrentCondition,
                                      LocalSystemMatrixType& rLHS_Contribution,
                                      LocalSystemVectorType& rRHS_Contribution,
                                      Condition::EquationIdVectorType& rEquationId,
                                      const ProcessInfo& rCurrentProcessInfo) override
    {
        KRATOS_TRY;

        const auto k = OpenMPUtils::ThisThread();
        rCurrentCondition.GetValuesVector(mAdjointValuesVector[k]);
        const auto local_size = mAdjointValuesVector[k].size();
        if (rRHS_Contribution.size() != local_size)
        {
            rRHS_Contribution.resize(local_size, false);
        }
        if (rLHS_Contribution.size1() != local_size || rLHS_Contribution.size2() != local_size)
        {
            rLHS_Contribution.resize(local_size, local_size, false);
        }
        this->CheckAndResizeThreadStorage(local_size);

        this->CalculateGradientContributions(rCurrentCondition, rLHS_Contribution,
                                             rRHS_Contribution, rCurrentProcessInfo);

        this->CalculateFirstDerivativeContributions(
            rCurrentCondition, rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);

        this->CalculateSecondDerivativeContributions(
            rCurrentCondition, rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);

        this->CalculateResidualLocalContributions(
            rCurrentCondition, rLHS_Contribution, rRHS_Contribution, rCurrentProcessInfo);

        rCurrentCondition.EquationIdVector(rEquationId, rCurrentProcessInfo);

        KRATOS_CATCH("");
    }

    void CalculateLHSContribution(Condition& rCurrentCondition,
                                  LocalSystemMatrixType& rLHS_Contribution,
                                  Condition::EquationIdVectorType& rEquationId,
                                  const ProcessInfo& rCurrentProcessInfo) override
    {
        KRATOS_TRY;
        LocalSystemVectorType RHS_Contribution;
        CalculateSystemContributions(rCurrentCondition,
                                     rLHS_Contribution, RHS_Contribution,
                                     rEquationId, rCurrentProcessInfo);
        KRATOS_CATCH("");
    }

    void Clear() override
    {
        this->mpDofUpdater->Clear();
    }

    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Inquiry
    ///@{

    ///@}
    ///@name Input and output
    ///@{

    /// Turn back information as a string.
    std::string Info() const override
    {
        return "ResidualBasedAdjointBossakScheme";
    }

    /// Print information about this object.
    void PrintInfo(std::ostream& rOStream) const override
    {
        rOStream << Info();
    }

    /// Print object's data.
    void PrintData(std::ostream& rOStream) const override
    {
        rOStream << Info();
    }
    ///@}
    ///@name Friends
    ///@{

    ///@}

protected:
    ///@name Protected static Member Variables
    ///@{

    ///@}
    ///@name Protected member Variables
    ///@{

    ///@}
    ///@name Protected Operators
    ///@{

    ///@}
    ///@name Protected Operations
    ///@{

    ///@}
    ///@name Protected  Access
    ///@{

    ///@}
    ///@name Protected Inquiry
    ///@{

    ///@}
    ///@name Protected LifeCycle
    ///@{

    ///@}

private:
    struct BossakConstants
    {
        double Alpha;
        double Beta;
        double Gamma;
        double C0;
        double C1;
        double C2;
        double C3;
        double C4;
        double C5;
        double C6;
        double C7;
    };

    ///@name Static Member Variables
    ///@{

    ///@}
    ///@name Member Variables
    ///@{

    BossakConstants mBossak;
    typename TSparseSpace::DofUpdaterPointerType mpDofUpdater =
        TSparseSpace::CreateDofUpdater();
    AdjointResponseFunction::Pointer mpResponseFunction;

    std::vector<LocalSystemMatrixType> mLeftHandSide;
    std::vector<LocalSystemVectorType> mResponseGradient;
    std::vector<LocalSystemMatrixType> mFirstDerivsLHS;
    std::vector<LocalSystemVectorType> mFirstDerivsResponseGradient;
    std::vector<LocalSystemMatrixType> mSecondDerivsLHS;
    std::vector<LocalSystemVectorType> mSecondDerivsResponseGradient;
    std::vector<LocalSystemVectorType> mAdjointValuesVector;
    std::vector<std::vector<IndirectScalar<double>>> mAdjointIndirectVector2;
    std::vector<std::vector<IndirectScalar<double>>> mAdjointIndirectVector3;
    std::vector<std::vector<IndirectScalar<double>>> mAuxAdjointIndirectVector1;

    ///@}
    ///@name Private Operators
    ///@{

    ///@}
    ///@name Private Operations
    ///@{

    /**
     * @brief Calculates entity first derivative contributions for adjoint system
     *
     * \[
     *      \mathbf{\underline{K}} = \mathbf{\underline{K}} + \frac{\partial \underline{R}^n}{\partial \underline{w}^n} \\
     *      \underline{F} = \underline{F} - \frac{\partial J^n}{\partial \underline{w}^n}
     * \]
     *
     * @tparam TEntityType
     * @param rCurrentEntity            Current entity
     * @param rLHS_Contribution         Left hand side matrix (i.e. $\mathbf{\underline{K}}$)
     * @param rRHS_Contribution         Right hand side vector (i.e. $\underline{F}$)
     * @param rCurrentProcessInfo       Current process info
     */
    template<class TEntityType>
    void CalculateGradientContributions(TEntityType& rCurrentEntity,
                                        LocalSystemMatrixType& rLHS_Contribution,
                                        LocalSystemVectorType& rRHS_Contribution,
                                        const ProcessInfo& rCurrentProcessInfo)
    {
        int k = OpenMPUtils::ThisThread();
        rCurrentEntity.CalculateLeftHandSide(mLeftHandSide[k], rCurrentProcessInfo);
        this->mpResponseFunction->CalculateGradient(
            rCurrentEntity, mLeftHandSide[k], mResponseGradient[k], rCurrentProcessInfo);
        noalias(rLHS_Contribution) = mLeftHandSide[k];
        noalias(rRHS_Contribution) = -1. * mResponseGradient[k];
    }

    /**
     * @brief Calculates element first derivative contributions to adjoint system
     *
     * \[
     *      \mathbf{\underline{K}} =  \mathbf{\underline{K}} + \frac{\gamma}{\beta \Delta t} \frac{\partial \underline{R}^n}{\partial \underline{\dot{w}}^n} \\
     *      \underline{F} = \underline{F} - \frac{\gamma}{\beta \Delta t} \frac{\partial J^n}{\partial \underline{\dot{w}}^n}
     * \]
     *
     * @tparam TEntityType
     * @param rCurrentEntity            Current entity
     * @param rLHS_Contribution         Left hand side matrix (i.e. $\mathbf{\underline{K}}$)
     * @param rRHS_Contribution         Right hand side vector (i.e. $\underline{F}$)
     * @param rCurrentProcessInfo       Current process info
     */
    template<class TEntityType>
    void CalculateFirstDerivativeContributions(TEntityType& rCurrentEntity,
                                               LocalSystemMatrixType& rLHS_Contribution,
                                               LocalSystemVectorType& rRHS_Contribution,
                                               const ProcessInfo& rCurrentProcessInfo)
    {
        int k = OpenMPUtils::ThisThread();
        rCurrentEntity.CalculateFirstDerivativesLHS(mFirstDerivsLHS[k], rCurrentProcessInfo);
        mpResponseFunction->CalculateFirstDerivativesGradient(
            rCurrentEntity, mFirstDerivsLHS[k],
            mFirstDerivsResponseGradient[k], rCurrentProcessInfo);
        noalias(rLHS_Contribution) += mBossak.C6 * mFirstDerivsLHS[k];
        noalias(rRHS_Contribution) -=
            mBossak.C6 * mFirstDerivsResponseGradient[k];
    }

    /**
     * @brief Calculates element second derivative contributions for adjoint system
     *
     * \[
     *      \mathbf{\underline{K}} =  \mathbf{\underline{K}} + \frac{1 - \alpha}{\beta\Delta t^2}\frac{\partial \underline{R}^n}{\partial \underline{\ddot{w}}^n} \\
     *      \underline{F} = \underline{F} - \frac{1}{\beta\Delta t^2}\frac{\partial J^n}{\partial \underline{\ddot{w}}^n}
     * \]
     *
     * @tparam TEntityType
     * @param rCurrentEntity            Current entity
     * @param rLHS_Contribution         Left hand side matrix (i.e. $\mathbf{\underline{K}}$)
     * @param rRHS_Contribution         Right hand side vector (i.e. $\underline{F}$)
     * @param rCurrentProcessInfo       Current process info
     */
    template<class TEntityType>
    void CalculateSecondDerivativeContributions(TEntityType& rCurrentEntity,
                                                LocalSystemMatrixType& rLHS_Contribution,
                                                LocalSystemVectorType& rRHS_Contribution,
                                                const ProcessInfo& rCurrentProcessInfo)
    {
        int k = OpenMPUtils::ThisThread();
        auto& r_response_function = *(this->mpResponseFunction);
        rCurrentEntity.CalculateSecondDerivativesLHS(mSecondDerivsLHS[k], rCurrentProcessInfo);
        mSecondDerivsLHS[k] *= (1.0 - mBossak.Alpha);
        r_response_function.CalculateSecondDerivativesGradient(
            rCurrentEntity, mSecondDerivsLHS[k],
            mSecondDerivsResponseGradient[k], rCurrentProcessInfo);
        noalias(rLHS_Contribution) += mBossak.C7 * mSecondDerivsLHS[k];
        noalias(rRHS_Contribution) -=
            mBossak.C7 * mSecondDerivsResponseGradient[k];
    }

    /**
     * @brief Calculates previous time step contributions from elements to adjoint system
     *
     * No need to use again conditions version of this since elements includes condition nodes as well.
     * Therefore, this will add automatically condition contributions as well.
     *
     * \underline{F} =
     *      \underline{F}
     *      - \frac{1}{\beta\Delta t^2}\left[\frac{\partial \underline{R}^{n+1}}{\underline{\ddot{w}}^n}\right]^T\underline{\lambda}_1^{n+1}
     *      - \frac{1}{\beta\Delta t^2}\frac{\partial J^{n+1}}{\underline{\ddot{w}}^n}
     *      + \frac{\beta - \gamma\left(\gamma + \frac{1}{2}\right)}{\beta^2\Delta t}\underline{\lambda}_2^{n+1}
     *      - \frac{\gamma + \frac{1}{2}}{\beta^2\Delta t}\underline{\lambda}_3^{n+1}
     *
     * @param rCurrentElement           Current element
     * @param rLHS_Contribution         Left hand side matrix (i.e. $\mathbf{\underline{K}}$)
     * @param rRHS_Contribution         Right hand side vector (i.e. $\underline{F}$)
     * @param rCurrentProcessInfo       Current process info
     */
    void CalculatePreviousTimeStepContributions(Element& rCurrentElement,
                                                LocalSystemMatrixType& rLHS_Contribution,
                                                LocalSystemVectorType& rRHS_Contribution,
                                                const ProcessInfo& rCurrentProcessInfo)
    {
        const auto& r_geometry = rCurrentElement.GetGeometry();
        const auto k = OpenMPUtils::ThisThread();
        auto& r_extensions = *rCurrentElement.GetValue(ADJOINT_EXTENSIONS);

        unsigned local_index = 0;
        for (unsigned i_node = 0; i_node < r_geometry.PointsNumber(); ++i_node)
        {
            auto& r_node = r_geometry[i_node];
            r_extensions.GetFirstDerivativesVector(i_node, mAdjointIndirectVector2[k], 1);
            r_extensions.GetSecondDerivativesVector(i_node, mAdjointIndirectVector3[k], 1);
            r_extensions.GetAuxiliaryVector(i_node, mAuxAdjointIndirectVector1[k], 1);
            const double weight = 1.0 / r_node.GetValue(NUMBER_OF_NEIGHBOUR_ELEMENTS);

            for (unsigned d = 0; d < mAdjointIndirectVector2[k].size(); ++d)
            {
                rRHS_Contribution[local_index] +=
                    weight *
                    (mBossak.C7 * mAuxAdjointIndirectVector1[k][d] +
                     mBossak.C4 * mAdjointIndirectVector2[k][d] +
                     mBossak.C5 * mAdjointIndirectVector3[k][d]);
                ++local_index;
            }
        }
    }

    /**
     * @brief Calculates elemental residual
     *
     * \[
     *      \underline{F} = \underline{F} - \mathbf{\underline{K}}\underline{\lambda}_1
     * \]
     *
     * @tparam TEntityType
     * @param rCurrentEntity            Current entity
     * @param rLHS_Contribution         Left hand side matrix (i.e. $\mathbf{\underline{K}}$)
     * @param rRHS_Contribution         Right hand side vector (i.e. $\underline{F}$)
     * @param rCurrentProcessInfo       Current process info
     */
    template<class TEntityType>
    void CalculateResidualLocalContributions(TEntityType& rCurrentEntity,
                                             LocalSystemMatrixType& rLHS_Contribution,
                                             LocalSystemVectorType& rRHS_Contribution,
                                             const ProcessInfo& rCurrentProcessInfo)
    {
        int k = OpenMPUtils::ThisThread();
        auto& r_residual_adjoint = mAdjointValuesVector[k];
        rCurrentEntity.GetValuesVector(r_residual_adjoint);
        noalias(rRHS_Contribution) -= prod(rLHS_Contribution, r_residual_adjoint);
    }

    void CalculateNodeNeighbourCount(ModelPart& rModelPart)
    {
        // Calculate number of neighbour elements for each node.
        VariableUtils().SetNonHistoricalVariableToZero(NUMBER_OF_NEIGHBOUR_ELEMENTS, rModelPart.Nodes());

        block_for_each(rModelPart.Elements(), [&](ModelPart::ElementType& rElement) {
            auto& r_geometry = rElement.GetGeometry();
            for (unsigned j = 0; j < r_geometry.PointsNumber(); ++j) {
                double& r_num_neighbour =
                    r_geometry[j].GetValue(NUMBER_OF_NEIGHBOUR_ELEMENTS);
#pragma omp atomic
                r_num_neighbour += 1.0;
            }
        });

        rModelPart.GetCommunicator().AssembleNonHistoricalData(NUMBER_OF_NEIGHBOUR_ELEMENTS);
    }

    void UpdateTimeSchemeAdjoints(ModelPart& rModelPart)
    {
        KRATOS_TRY;
        auto lambda2_vars = GatherVariables(
            rModelPart.Elements(), [](const AdjointExtensions& rExtensions,
                                      std::vector<const VariableData*>& rVec) {
                rExtensions.GetFirstDerivativesVariables(rVec);
            });
        auto lambda3_vars = GatherVariables(
            rModelPart.Elements(), [](const AdjointExtensions& rExtensions,
                                      std::vector<const VariableData*>& rVec) {
                return rExtensions.GetSecondDerivativesVariables(rVec);
            });
        auto auxiliary_vars = GatherVariables(
            rModelPart.Elements(), [](const AdjointExtensions& rExtensions,
                                      std::vector<const VariableData*>& rVec) {
                return rExtensions.GetAuxiliaryVariables(rVec);
            });

        SetToZero_AdjointVars(lambda2_vars, rModelPart.Nodes());
        SetToZero_AdjointVars(lambda3_vars, rModelPart.Nodes());

        const auto& r_process_info = rModelPart.GetProcessInfo();
        CalculateAndUpdateEntityTimeSchemeContributions(rModelPart.Elements(), r_process_info);
        CalculateAndUpdateEntityTimeSchemeContributions(rModelPart.Conditions(), r_process_info);

        KRATOS_ERROR_IF(lambda2_vars.size() != lambda3_vars.size())
            << "First derivatives variable list and second derivatives "
               "variables list size mismatch.\n";
        KRATOS_ERROR_IF(lambda2_vars.size() != auxiliary_vars.size())
            << "First derivatives variable list and auxiliary variables list "
               "size mismatch.\n";

        for (unsigned int i_var = 0; i_var < lambda2_vars.size(); ++i_var) {
            const auto& r_lambda2_variable_name = lambda2_vars[i_var]->Name();
            const auto& r_lambda3_variable_name = lambda3_vars[i_var]->Name();
            const auto& r_auxiliary_variable_name = auxiliary_vars[i_var]->Name();

            if (KratosComponents<Variable<array_1d<double, 3>>>::Has(r_lambda2_variable_name)) {
                UpdateTimeSchemeVariablesFromOldContributions<array_1d<double, 3>>(
                    rModelPart.Nodes(), r_lambda2_variable_name,
                    r_lambda3_variable_name, r_auxiliary_variable_name);
            } else if (KratosComponents<Variable<double>>::Has(r_lambda2_variable_name)) {
                UpdateTimeSchemeVariablesFromOldContributions<double>(
                    rModelPart.Nodes(), r_lambda2_variable_name,
                    r_lambda3_variable_name, r_auxiliary_variable_name);
            } else {
                KRATOS_ERROR << "Unsupported variable type "
                             << r_lambda2_variable_name << ".";
            }
        }

        // Finalize global assembly
        Assemble_AdjointVars(lambda2_vars, rModelPart.GetCommunicator());
        Assemble_AdjointVars(lambda3_vars, rModelPart.GetCommunicator());
        KRATOS_CATCH("");
    }

    template <class TEntityContainerType>
    void CalculateAndUpdateEntityTimeSchemeContributions(
        TEntityContainerType& rEntityContainer,
        const ProcessInfo& rProcessInfo)
    {
        KRATOS_TRY

        const int number_of_elements = rEntityContainer.size();

        Vector adjoint2_aux, adjoint3_aux;
        std::vector<IndirectScalar<double>> adjoint2_old, adjoint3_old;
#pragma omp parallel for private(adjoint2_aux, adjoint3_aux, adjoint2_old, adjoint3_old)
        for (int i = 0; i < number_of_elements; ++i) {
            auto& r_entity = *(rEntityContainer.begin() + i);
            const int k = OpenMPUtils::ThisThread();

            r_entity.GetValuesVector(mAdjointValuesVector[k]);
            this->CheckAndResizeThreadStorage(mAdjointValuesVector[k].size());

            /// starting to build residual for next time step calculations
            r_entity.CalculateFirstDerivativesLHS(mFirstDerivsLHS[k], rProcessInfo);
            this->mpResponseFunction->CalculateFirstDerivativesGradient(
                r_entity, mFirstDerivsLHS[k], mFirstDerivsResponseGradient[k], rProcessInfo);

            r_entity.CalculateSecondDerivativesLHS(mSecondDerivsLHS[k], rProcessInfo);
            mSecondDerivsLHS[k] *= (1.0 - mBossak.Alpha);
            this->mpResponseFunction->CalculateSecondDerivativesGradient(
                r_entity, mSecondDerivsLHS[k], mSecondDerivsResponseGradient[k], rProcessInfo);

            if (adjoint2_aux.size() != mFirstDerivsResponseGradient[k].size())
                adjoint2_aux.resize(mFirstDerivsResponseGradient[k].size(), false);
            noalias(adjoint2_aux) = -mFirstDerivsResponseGradient[k] -
                                    prod(mFirstDerivsLHS[k], mAdjointValuesVector[k]);
            if (adjoint3_aux.size() != mSecondDerivsResponseGradient[k].size())
                adjoint3_aux.resize(mSecondDerivsResponseGradient[k].size(), false);
            noalias(adjoint3_aux) = -mSecondDerivsResponseGradient[k] -
                                    prod(mSecondDerivsLHS[k], mAdjointValuesVector[k]);
            auto& r_extensions = *r_entity.GetValue(ADJOINT_EXTENSIONS);

            // Assemble the contributions to the corresponding nodal unknowns.
            unsigned local_index = 0;
            auto& r_geometry = r_entity.GetGeometry();
            for (unsigned i_node = 0; i_node < r_geometry.PointsNumber(); ++i_node) {

                r_extensions.GetFirstDerivativesVector(i_node, mAdjointIndirectVector2[k], 0);
                r_extensions.GetSecondDerivativesVector(i_node, mAdjointIndirectVector3[k], 0);

                auto& r_node = r_geometry[i_node];
                r_node.SetLock();
                for (unsigned d = 0; d < mAdjointIndirectVector2[k].size(); ++d) {
                    /*
                        \[
                            mAdjointIndirectVector2 =
                                - \frac{\partial J^{n}}{\partial \underline{\dot{w}}^n}
                                - \left[\frac{\partial \underline{R}^{n}}{\partial \underline{\dot{w}}}\right]^T\underline{\lambda}_1^{n+1}
                        \]
                        \[
                            mAdjointIndirectVector3 =
                                - \frac{\partial J^{n}}{\partial \underline{\ddot{w}}^n}
                                - \left(1-\alpha\right)\left[\frac{\partial \underline{R}^{n}}{\partial \underline{\ddot{w}}^n}\right]^T\underline{\lambda}_1^{n+1}
                        \]
                    */
                    mAdjointIndirectVector2[k][d] += adjoint2_aux[local_index];
                    mAdjointIndirectVector3[k][d] += adjoint3_aux[local_index];
                    ++local_index;
                }
                r_node.UnSetLock();
            }
        }

        KRATOS_CATCH("");
    }

    template<class TDataType>
    void UpdateTimeSchemeVariablesFromOldContributions(
        ModelPart::NodesContainerType& rNodes,
        const std::string& rLambda2VariableName,
        const std::string& rLambda3VariableName,
        const std::string& rAuxiliaryVariableName)
    {
        KRATOS_TRY

        KRATOS_ERROR_IF(!KratosComponents<Variable<TDataType>>::Has(rLambda2VariableName))
            << "Adjoint variable " << rLambda2VariableName
            << " is not found in variable list with required type.\n";

        KRATOS_ERROR_IF(!KratosComponents<Variable<TDataType>>::Has(rLambda3VariableName))
            << "Adjoint variable " << rLambda3VariableName
            << " is not found in variable list with required type.\n";

        KRATOS_ERROR_IF(!KratosComponents<Variable<TDataType>>::Has(rAuxiliaryVariableName))
            << "Adjoint variable " << rAuxiliaryVariableName
            << " is not found in variable list with required type.\n";

        const auto& r_lambda2_variable = KratosComponents<Variable<TDataType>>::Get(rLambda2VariableName);
        const auto& r_lambda3_variable = KratosComponents<Variable<TDataType>>::Get(rLambda3VariableName);
        const auto& r_auxiliary_variable = KratosComponents<Variable<TDataType>>::Get(rAuxiliaryVariableName);

        block_for_each(rNodes, [&](ModelPart::NodeType& rNode) {
            const TDataType& r_old_lambda2_value = rNode.FastGetSolutionStepValue(r_lambda2_variable, 1);
            const TDataType& r_old_lambda3_value = rNode.FastGetSolutionStepValue(r_lambda3_variable, 1);

            TDataType& r_lambda2_value = rNode.FastGetSolutionStepValue(r_lambda2_variable);
            r_lambda2_value += r_old_lambda2_value * mBossak.C0;
            r_lambda2_value += r_old_lambda3_value * mBossak.C1;

            TDataType& r_lambda3_value = rNode.FastGetSolutionStepValue(r_lambda3_variable);
            r_lambda3_value += r_old_lambda2_value * mBossak.C2;
            r_lambda3_value += r_old_lambda3_value * mBossak.C3;
            r_lambda3_value += rNode.FastGetSolutionStepValue(r_auxiliary_variable, 1);
        });

        KRATOS_CATCH("");
    }

    void UpdateAuxiliaryVariable(ModelPart& rModelPart)
    {
        KRATOS_TRY;
        auto aux_vars = GatherVariables(
            rModelPart.Elements(), [](const AdjointExtensions& rExtensions,
                                      std::vector<const VariableData*>& rOut) {
                return rExtensions.GetAuxiliaryVariables(rOut);
            });

        SetToZero_AdjointVars(aux_vars, rModelPart.Nodes());

        const auto& r_process_info = rModelPart.GetProcessInfo();
        // Loop over elements to assemble the remaining terms
        CalculateAndUpdateEntityAuxiliaryVariableContributions(rModelPart.Elements(), r_process_info);
        // Loop over conditions to assemble the remaining terms
        CalculateAndUpdateEntityAuxiliaryVariableContributions(rModelPart.Conditions(), r_process_info);

        // Finalize global assembly
        Assemble_AdjointVars(aux_vars, rModelPart.GetCommunicator());
        KRATOS_CATCH("");
    }

    template <class TEntityContainerType>
    void CalculateAndUpdateEntityAuxiliaryVariableContributions(
        TEntityContainerType& rEntityContainer,
        const ProcessInfo& rProcessInfo)
    {
        KRATOS_TRY

        const int number_of_entities = rEntityContainer.size();
        Vector aux_adjoint_vector;
#pragma omp parallel for private(aux_adjoint_vector)
        for (int i = 0; i < number_of_entities; ++i) {
            auto& r_entity = *(rEntityContainer.begin() + i);
            const int k = OpenMPUtils::ThisThread();

            r_entity.GetValuesVector(mAdjointValuesVector[k]);
            this->CheckAndResizeThreadStorage(mAdjointValuesVector[k].size());

            r_entity.CalculateSecondDerivativesLHS(mSecondDerivsLHS[k], rProcessInfo);
            mSecondDerivsLHS[k] *= mBossak.Alpha;
            this->mpResponseFunction->CalculateSecondDerivativesGradient(
                r_entity, mSecondDerivsLHS[k], mSecondDerivsResponseGradient[k], rProcessInfo);

            if (aux_adjoint_vector.size() != mSecondDerivsLHS[k].size1())
                aux_adjoint_vector.resize(mSecondDerivsLHS[k].size1(), false);
            noalias(aux_adjoint_vector) =
                prod(mSecondDerivsLHS[k], mAdjointValuesVector[k]) +
                mSecondDerivsResponseGradient[k];
            auto& r_extensions = *r_entity.GetValue(ADJOINT_EXTENSIONS);
            // Assemble the contributions to the corresponding nodal unknowns.
            unsigned local_index = 0;
            auto& r_geometry = r_entity.GetGeometry();
            /*
                mAuxAdjointIndirectVector1 =
                    - \frac{\partial J^{n+1}}{\partial \underline{\ddot{w}}^n}
                    - \alpha \left[\frac{\partial \underline{R}^{n+1}}{\partial \underline{\ddot{w}}^n}\right]^T\underline{\lambda}_1^{n+1}
            */
            for (unsigned i_node = 0; i_node < r_geometry.PointsNumber(); ++i_node) {
                auto& r_node = r_geometry[i_node];
                r_extensions.GetAuxiliaryVector(i_node, mAuxAdjointIndirectVector1[k], 0);

                r_node.SetLock();
                for (unsigned d = 0; d < mAuxAdjointIndirectVector1[k].size(); ++d) {
                    mAuxAdjointIndirectVector1[k][d] -= aux_adjoint_vector[local_index];
                    ++local_index;
                }
                r_node.UnSetLock();
            }
        }

        KRATOS_CATCH("");
    }

    void CheckAndResizeThreadStorage(unsigned SystemSize)
    {
        const int k = OpenMPUtils::ThisThread();

        if (mLeftHandSide[k].size1() != SystemSize || mLeftHandSide[k].size2() != SystemSize)
        {
            mLeftHandSide[k].resize(SystemSize, SystemSize, false);
        }

        if (mFirstDerivsLHS[k].size1() != SystemSize || mFirstDerivsLHS[k].size2() != SystemSize)
        {
            mFirstDerivsLHS[k].resize(SystemSize, SystemSize, false);
        }

        if (mSecondDerivsLHS[k].size1() != SystemSize || mSecondDerivsLHS[k].size2() != SystemSize)
        {
            mSecondDerivsLHS[k].resize(SystemSize, SystemSize, false);
        }

        if (mResponseGradient[k].size() != SystemSize)
        {
            mResponseGradient[k].resize(SystemSize, false);
        }

        if (mFirstDerivsResponseGradient[k].size() != SystemSize)
        {
            mFirstDerivsResponseGradient[k].resize(SystemSize, false);
        }

        if (mSecondDerivsResponseGradient[k].size() != SystemSize)
        {
            mSecondDerivsResponseGradient[k].resize(SystemSize, false);
        }
    }

    static BossakConstants CalculateBossakConstants(double Alpha, double DeltaTime)
    {
        BossakConstants bc;
        bc.Alpha = Alpha;
        bc.Beta = 0.25 * (1.0 - bc.Alpha) * (1.0 - bc.Alpha);
        bc.Gamma = 0.5 - bc.Alpha;
        bc.C0 = 1.0 - bc.Gamma / bc.Beta;
        bc.C1 = -1.0 / (bc.Beta * DeltaTime);
        bc.C2 = (1.0 - 0.5 * bc.Gamma / bc.Beta) * DeltaTime;
        bc.C3 = (1.0 - 0.5 / bc.Beta);
        bc.C4 = (bc.Beta - bc.Gamma * (bc.Gamma + 0.5)) / (DeltaTime * bc.Beta * bc.Beta);
        bc.C5 = -1.0 * (bc.Gamma + 0.5) / (DeltaTime * DeltaTime * bc.Beta * bc.Beta);
        bc.C6 = bc.Gamma / (bc.Beta * DeltaTime);
        bc.C7 = 1.0 / (DeltaTime * DeltaTime * bc.Beta);
        return bc;
    }

    static double GetTimeStep(const ProcessInfo& rCurrentProcessInfo)
    {
        const ProcessInfo& r_last_process_info =
            rCurrentProcessInfo.GetPreviousSolutionStepInfo(1);

        // Note: solution is backwards in time, but we still want a positive
        // time step
        // (it is the time step in the "forward" Bossak scheme).
        double time_step =
            r_last_process_info.GetValue(TIME) - rCurrentProcessInfo.GetValue(TIME);
        KRATOS_ERROR_IF(time_step <= 0.0)
            << "Backwards in time solution is not decreasing time from last "
               "step."
            << std::endl;
        return time_step;
    }

    struct Hash
    {
        std::size_t operator()(const VariableData* const& p) const
        {
            return p->Key();
        }
    };

    struct Pred
    {
        bool operator()(const VariableData* const l, const VariableData* const r) const
        {
            return *l == *r;
        }
    };

    // Gathers variables needed for assembly.
    static std::vector<const VariableData*> GatherVariables(
        const ModelPart::ElementsContainerType& rElements,
        std::function<void(const AdjointExtensions&, std::vector<const VariableData*>&)> GetLocalVars)
    {
        KRATOS_TRY;
        const int num_threads = OpenMPUtils::GetNumThreads();
        std::vector<const VariableData*> local_vars;
        std::vector<std::unordered_set<const VariableData*, Hash, Pred>> thread_vars(num_threads);
#pragma omp parallel for private(local_vars)
        for (int i = 0; i < static_cast<int>(rElements.size()); ++i)
        {
            auto& r_element = *(rElements.begin() + i);
            GetLocalVars(*r_element.GetValue(ADJOINT_EXTENSIONS), local_vars);
            const int k = OpenMPUtils::ThisThread();
            thread_vars[k].insert(local_vars.begin(), local_vars.end());
        }
        std::unordered_set<const VariableData*, Hash, Pred> all_vars;
        for (int i = 0; i < num_threads; ++i)
        {
            all_vars.insert(thread_vars[i].begin(), thread_vars[i].end());
        }
        return std::vector<const VariableData*>{all_vars.begin(), all_vars.end()};
        KRATOS_CATCH("");
    }

    static void SetToZero_AdjointVars(const std::vector<const VariableData*>& rVariables,
                                      ModelPart::NodesContainerType& rNodes)
    {
        KRATOS_TRY;
        for (auto p_variable_data : rVariables)
        {
            if (KratosComponents<Variable<array_1d<double, 3>>>::Has(
                    p_variable_data->Name()))
            {
                const auto& r_variable =
                    KratosComponents<Variable<array_1d<double, 3>>>::Get(
                        p_variable_data->Name());
                VariableUtils().SetHistoricalVariableToZero(r_variable, rNodes);
            }
            else if (KratosComponents<Variable<double>>::Has(p_variable_data->Name()))
            {
                const auto& r_variable =
                    KratosComponents<Variable<double>>::Get(p_variable_data->Name());
                VariableUtils().SetHistoricalVariableToZero(r_variable, rNodes);
            }
            else
            {
                KRATOS_ERROR << "Variable \"" << p_variable_data->Name()
                             << "\" not found!\n";
            }
        }
        KRATOS_CATCH("");
    }

    static void Assemble_AdjointVars(const std::vector<const VariableData*>& rVariables,
                                     Communicator& rComm)
    {
        KRATOS_TRY;
        for (auto p_variable_data : rVariables)
        {
            if (KratosComponents<Variable<array_1d<double, 3>>>::Has(
                    p_variable_data->Name()))
            {
                const auto& r_variable =
                    KratosComponents<Variable<array_1d<double, 3>>>::Get(
                        p_variable_data->Name());
                rComm.AssembleCurrentData(r_variable);
            }
            else if (KratosComponents<Variable<double>>::Has(p_variable_data->Name()))
            {
                const auto& r_variable =
                    KratosComponents<Variable<double>>::Get(p_variable_data->Name());
                rComm.AssembleCurrentData(r_variable);
            }
            else
            {
                KRATOS_ERROR << "Variable \"" << p_variable_data->Name()
                             << "\" not found!\n";
            }
        }
        KRATOS_CATCH("");
    }

    ///@}
    ///@name Private  Access
    ///@{

    ///@}
    ///@name Private Inquiry
    ///@{

    ///@}
    ///@name Un accessible methods
    ///@{

    ///@}

}; /* Class ResidualBasedAdjointBossakScheme */

///@}

///@name Type Definitions
///@{

///@}

} /* namespace Kratos.*/

#endif /* KRATOS_RESIDUAL_BASED_ADJOINT_BOSSAK_SCHEME_H_INCLUDED defined */
