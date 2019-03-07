// KRATOS  ___|  |                   |                   |
//       \___ \  __|  __| |   |  __| __| |   |  __| _` | |
//             | |   |    |   | (    |   |   | |   (   | |
//       _____/ \__|_|   \__,_|\___|\__|\__,_|_|  \__,_|_| MECHANICS
//
//  License:		 BSD License
//					 license: structural_mechanics_application/license.txt
//
//  Main authors:    Martin Fusseder, https://github.com/MFusseder
//


// Project includes
#include "utilities/compare_elements_and_conditions_utility.h"
#include "replace_elements_and_conditions_for_adjoint_problem_process.h"
#include "structural_mechanics_application_variables.h"
// #include "custom_response_functions/adjoint_elements/adjoint_finite_difference_base_element.h"
// #include "custom_response_functions/adjoint_elements/adjoint_finite_difference_shell_element.h"
// #include "custom_response_functions/adjoint_elements/adjoint_finite_difference_cr_beam_element_3D2N.h"
// #include "custom_response_functions/adjoint_elements/adjoint_finite_difference_truss_element_3D2N.h"
// #include "custom_response_functions/adjoint_elements/adjoint_finite_difference_truss_element_linear_3D2N.h"
// #include "custom_response_functions/adjoint_conditions/adjoint_semi_analytic_base_condition.h"
// #include "custom_response_functions/adjoint_conditions/adjoint_semi_analytic_point_load_condition.h"

#include "custom_response_functions/adjoint_processes/replace_multiple_elements_and_conditions_process.h"

namespace Kratos
{


    ReplaceElementsAndConditionsForAdjointProblemProcess::ReplaceElementsAndConditionsForAdjointProblemProcess(ModelPart& model_part) : Process(Flags()) , mrModelPart(model_part)
    {
        KRATOS_TRY

        KRATOS_CATCH("")
    }

    void ReplaceElementsAndConditionsForAdjointProblemProcess::Execute()
    {
        KRATOS_ERROR_IF(mrModelPart.IsSubModelPart()) << "The replacement process can only be done for the root model part!" << std::endl;

        if ( (!mrModelPart.GetProcessInfo().Has(IS_ADJOINT)) || (!mrModelPart.GetProcessInfo()[IS_ADJOINT]) )
        {
            this->ReplaceToAdjoint();
            mrModelPart.GetProcessInfo()[IS_ADJOINT] = true;
        }
    }


    void ReplaceElementsAndConditionsForAdjointProblemProcess::ReplaceToAdjoint()
    {


        Parameters parameters = Parameters(R"(
        {
            "element_name_table"    : {
                "ShellThinElement3D3N" : "AdjointFiniteDifferencingShellThinElement3D3N",
                "CrLinearBeamElement3D2N" : "AdjointFiniteDifferenceCrBeamElementLinear3D2N",
                "TrussLinearElement3D2N" : "AdjointFiniteDifferenceTrussLinearElement3D2N",
                "TrussElement3D2N" : "AdjointFiniteDifferenceTrussElement3D2N"
            },
            "condition_name_table"    : {
                "PointLoadCondition2D1N" : "AdjointSemiAnalyticPointLoadCondition2D1N",
                "PointLoadCondition3D1N": "AdjointSemiAnalyticPointLoadCondition3D1N"
            },
            "throw_error" : false
        })" );

        ReplaceMultipleElementsAndConditionsProcess replacement_process(mrModelPart, parameters);
        replacement_process.Execute();

    }

    /// Turn back information as a string.
    std::string ReplaceElementsAndConditionsForAdjointProblemProcess::Info() const
    {
        return "ReplaceElementsAndConditionsForAdjointProblemProcess";
    }

    /// Print information about this object.
    void ReplaceElementsAndConditionsForAdjointProblemProcess::PrintInfo(std::ostream& rOStream) const
    {
        rOStream << "ReplaceElementsAndConditionsForAdjointProblemProcess";
    }

    /// Print object's data.
    void ReplaceElementsAndConditionsForAdjointProblemProcess::PrintData(std::ostream& rOStream) const
    {
    }

    void ReplaceElementsAndConditionsForAdjointProblemProcess::UpdateSubModelPart(ModelPart& r_model_part, ModelPart& r_root_model_part)
    {
        //change the model part itself
        #pragma omp parallel for
        for(int i=0; i<static_cast<int>(r_model_part.NumberOfElements()); ++i)
        {
            const auto it = r_model_part.ElementsBegin() + i;

            (*it.base()) = r_root_model_part.Elements()(it->Id());
        }

        #pragma omp parallel for
        for(int i=0; i<static_cast<int>(r_model_part.NumberOfConditions()); ++i)
        {
            const auto it = r_model_part.ConditionsBegin() + i;

            (*it.base()) = r_root_model_part.Conditions()(it->Id());
        }

        //change the sons
        for (ModelPart::SubModelPartIterator i_sub_model_part = r_model_part.SubModelPartsBegin(); i_sub_model_part != r_model_part.SubModelPartsEnd(); i_sub_model_part++)
            UpdateSubModelPart( *i_sub_model_part, r_root_model_part );
    }

    bool ReplaceElementsAndConditionsForAdjointProblemProcess::GetAdjointElementName(const Element& rElement, std::string& rName)
    {
        KRATOS_TRY

        bool replacement_necessary = true;
        std::string name_current_element;
        CompareElementsAndConditionsUtility::GetRegisteredName(rElement, name_current_element);

        // Add here all new adjoint elements or elements which should be ignored by the replacement process
        if(name_current_element == "CrLinearBeamElement3D2N")
            rName = "AdjointFiniteDifferenceCrBeamElement";
        else if(name_current_element == "ShellThinElement3D3N")
            rName = "AdjointFiniteDifferencingShellElement";
        else if(name_current_element == "TrussElement3D2N")
            rName = "AdjointFiniteDifferenceTrussElement";
        else if(name_current_element == "TrussLinearElement3D2N")
            rName = "AdjointFiniteDifferenceTrussLinearElement";
        else
        {
            KRATOS_ERROR << "It is not possible to replace the " << name_current_element <<
             " because there is no equivalent adjoint/primal element available." << std::endl;
        }

        return replacement_necessary;

        KRATOS_CATCH("")
    }

    bool ReplaceElementsAndConditionsForAdjointProblemProcess::GetAdjointConditionName(const Condition& rCondition, std::string& rName)
    {
        KRATOS_TRY

        bool replacement_necessary = true;
        std::string name_current_condition;
        CompareElementsAndConditionsUtility::GetRegisteredName(rCondition, name_current_condition);

        // Add here all new adjoint conditions or conditions which should be ignored by the replacement process
        if(name_current_condition == "PointLoadCondition2D1N")
            rName = "AdjointSemiAnalyticPointLoadCondition";
        else if(name_current_condition == "PointLoadCondition3D1N")
            rName = "AdjointSemiAnalyticPointLoadCondition";
        else if(name_current_condition == "ShapeOptimizationCondition3D3N")
            replacement_necessary = false;
        else if(name_current_condition == "ShapeOptimizationCondition3D4N")
            replacement_necessary = false;
        else
        {
            KRATOS_ERROR << "It is not possible to replace the " << name_current_condition <<
             " because there is no equivalent adjoint/primal condition available." << std::endl;
        }


        return replacement_necessary;

        KRATOS_CATCH("")
    }

}  // namespace Kratos.




