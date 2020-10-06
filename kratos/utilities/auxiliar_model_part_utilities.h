//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Vicente Mataix Ferrandiz
//
//

#if !defined(KRATOS_AUXILIAR_MODEL_PART_UTILITIES)
#define KRATOS_AUXILIAR_MODEL_PART_UTILITIES

// System includes

// External includes

// Project includes
#include "includes/serializer.h"
#include "includes/model_part.h"
#include "utilities/parallel_utilities.h"

namespace Kratos
{
///@name Kratos Globals
///@{

///@}
///@name Type Definitions
///@{

    /// The index type definition
    typedef std::size_t IndexType;

///@}
///@name  Enum's
///@{

enum class DataLocation {
    NodeHistorical,
    NodeNonHistorical,
    Element,
    Condition,
    ModelPart,
    ProcessInfo
};

///@}
///@name  Functions
///@{

/**
 * @class AuxiliarModelPartUtilities
 * @ingroup KratosCore
 * @brief This uility includes auxiliar methods not included in the model part to avoid increase more than necessary the API
 * @author Vicente Mataix Ferrandiz
 */
class KRATOS_API(KRATOS_CORE) AuxiliarModelPartUtilities
{
public:
    ///@name Type Definitions
    ///@{

    /// Counted pointer of AuxiliarModelPartUtilities
    KRATOS_CLASS_POINTER_DEFINITION( AuxiliarModelPartUtilities );

    ///@}
    ///@name Life Cycle
    ///@{

    /**
     * The default constructor
     */
    AuxiliarModelPartUtilities(ModelPart& rModelPart):
        mrModelPart(rModelPart)
    {
    }

    virtual ~AuxiliarModelPartUtilities()= default;

    ///@}
    ///@name Access
    ///@{

    ///@}
    ///@name Inquiry
    ///@{

    ///@}
    ///@name Input and output
    ///@{

    ///@}
    ///@name Friends
    ///@{

    ///@}
    ///@name Operations
    ///@{

    /**
     * @brief This method ensured that the properties of elements and conditions are on the model part (it does recursively in all model parts)
     * @param RemovePreviousProperties If we clear previous properties and ensure only the properties existing in the elements and conditions (true by default)
     */
    void RecursiveEnsureModelPartOwnsProperties(const bool RemovePreviousProperties = true);

    /**
     * @brief This method ensured that the properties of elements and conditions are on the model part
     * @param RemovePreviousProperties If we clear previous properties and ensure only the properties existing in the elements and conditions (true by default)
     */
    void EnsureModelPartOwnsProperties(const bool RemovePreviousProperties = true);

    /**
     * @brief Remove the element with given Id from mesh with ThisIndex in this modelpart and all its subs.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param ElementId The id of the element to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveElementAndBelongings(IndexType ElementId, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given element from mesh with ThisIndex in this modelpart and all its subs.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param rThisElement The reference of the element
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveElementAndBelongings(Element& rThisElement, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given element from mesh with ThisIndex in this modelpart and all its subs.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param pThisElement The pointer to the element to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveElementAndBelongings(Element::Pointer pThisElement, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove the element with given Id from mesh with ThisIndex in parents, itself and children.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param ElementId The id of the element to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveElementAndBelongingsFromAllLevels(IndexType ElementId, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given element from mesh with ThisIndex in parents, itself and children.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param rThisElement The reference of the element
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveElementAndBelongingsFromAllLevels(Element& rThisElement, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given element from mesh with ThisIndex in parents, itself and children.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param pThisElement The pointer to the element to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveElementAndBelongingsFromAllLevels(Element::Pointer pThisElement, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief  It erases all elements identified by "IdentifierFlag" by removing the pointer.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * Pointers are erased from this level downwards nodes will be automatically destructured when no pointer is left to them
     * @param IdentifierFlag The flag that identifies the entities to remove
     */
    void RemoveElementsAndBelongings(Flags IdentifierFlag = TO_ERASE);

    /**
     * @brief It erases all elements identified by "IdentifierFlag" by removing the pointer.
     * @details This method removes belonging entities too.
     * This means that if the element has nodes defining a condition, and the nodes defining that condition are removed the condition is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * Pointers are erased from this level downwards nodes will be automatically destructured when no pointer is left to them
     * @param IdentifierFlag The flag that identifies the entities to remove
     */
    void RemoveElementsAndBelongingsFromAllLevels(Flags IdentifierFlag = TO_ERASE);

    /**
     * @brief Remove the condition with given Id from mesh with ThisIndex in this modelpart and all its subs.
     * @details This method removes belonging entities too.
     * This means that if the condition has nodes defining an element, and the nodes defining that element are removed the element is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param ConditionId The ID of the condition to remove
     * @param IdentifierFlag The flag that identifies the entities to removeentities too
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveConditionAndBelongings(IndexType ConditionId, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given condition from mesh with ThisIndex in this modelpart and all its subs.
     * @details This method removes belonging entities too.
     * This means that if the condition has nodes defining an element, and the nodes defining that element are removed the element is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param rThisCondition The reference to the condition to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveConditionAndBelongings(Condition& ThisCondition, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given condition from mesh with ThisIndex in this modelpart and all its subs. This method removes belonging entities too
     * @details The method check that when removing the nodes doesn't affect to other entities
     * @param pThisCondition The pointer to the condition to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveConditionAndBelongings(Condition::Pointer pThisCondition, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove the condition with given Id from mesh with ThisIndex in parents, itself and children.
     * @details This method removes belonging entities too.
     * This means that if the condition has nodes defining an element, and the nodes defining that element are removed the element is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param ConditionId The ID of the condition to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveConditionAndBelongingsFromAllLevels(IndexType ConditionId, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given condition from mesh with ThisIndex in parents, itself and children.
     * @details This method removes belonging entities too.
     * This means that if the condition has nodes defining an element, and the nodes defining that element are removed the element is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param rThisCondition The reference to the condition to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveConditionAndBelongingsFromAllLevels(Condition& rThisCondition, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief Remove given condition from mesh with ThisIndex in parents, itself and children.
     * @details This method removes belonging entities too.
     * This means that if the condition has nodes defining an element, and the nodes defining that element are removed the element is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * @param pThisCondition The pointer to the condition to remove
     * @param IdentifierFlag The flag that identifies the entities to remove
     * @param ThisIndex The index of the mesh where remove the entity
     */
    void RemoveConditionAndBelongingsFromAllLevels(Condition::Pointer pThisCondition, Flags IdentifierFlag = TO_ERASE, IndexType ThisIndex = 0);

    /**
     * @brief It erases all conditions identified by "IdentifierFlag" by removing the pointer.
     * @details This method removes belonging entities too.
     * This means that if the condition has nodes defining an element, and the nodes defining that element are removed the element is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * Pointers are erased from this level downwards nodes will be automatically destructured when no pointer is left to them
     * @param IdentifierFlag The flag that identifies the entities to remove
     */
    void RemoveConditionsAndBelongings(Flags IdentifierFlag = TO_ERASE);

    /**
     * @brief It erases all conditions identified by "IdentifierFlag" by removing the pointer.
     * @details This method removes belonging entities too.
     * This means that if the condition has nodes defining an element, and the nodes defining that element are removed the element is removed too
     * Basically the method checks that when removing the nodes doesn't affect to other entities
     * Pointers are erased from this level downwards nodes will be automatically destructured when no pointer is left to them
     * @param IdentifierFlag The flag that identifies the entities to remove
     */
    void RemoveConditionsAndBelongingsFromAllLevels(Flags IdentifierFlag = TO_ERASE);


    //To export a Scalar data (Double)
    std::vector<double> GetVariableData(
        const Variable<double>& rVariable,
        const DataLocation DataLoc) const
    {
        std::vector<double> data;

        switch (DataLoc)
        {
        case (DataLocation::NodeHistorical):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfNodes());
            for(const auto& r_node : mrModelPart.Nodes()){
                data[counter++] = r_node.FastGetSolutionStepValue(rVariable);
            }
            break;
        }
        case (DataLocation::NodeNonHistorical):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfNodes());
            for(const auto& r_node : mrModelPart.Nodes()){
                data[counter++] = r_node.GetValue(rVariable);
            }
            break;
        }
        case (DataLocation::Element):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfElements());
            for(const auto& r_elem : mrModelPart.Elements()){
                data[counter++] = r_elem.GetValue(rVariable);
            }
            break;
        }
        case (DataLocation::Condition):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfConditions());
            for(const auto& r_cond : mrModelPart.Conditions()){
                data[counter++] = r_cond.GetValue(rVariable);
            }
            break;
        }
        case (DataLocation::ModelPart):{
            data.resize(1);
            data[0] = mrModelPart[rVariable];
            break;
        }
        case (DataLocation::ProcessInfo):{
            data.resize(1);
            data[0] = mrModelPart.GetProcessInfo()[rVariable];
            break;
        }
        default:{
            //Throw an error about invalid DataLocation
            KRATOS_ERROR << "unknown Datalocation" << std::endl;
            break;
        }

        }

        return data;
    }


    //To export a Vector data
    template<std::size_t TSize>
    std::vector<double> GetVariableData(
        const Variable<array_1d<double, TSize>>& rVariable,
        const DataLocation DataLoc) const
    {
        std::vector<double> data;

        switch (DataLoc)
        {
        case (DataLocation::NodeHistorical):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfNodes()*TSize);
            for(const auto& r_node : mrModelPart.Nodes()){
                const array_1d<double, TSize>& r_val = r_node.FastGetSolutionStepValue(rVariable);
                for(int dim = 0 ; dim < TSize ; dim++){
                    data[counter++] = r_val[dim];
                }
            }
            break;
        }
        case (DataLocation::NodeNonHistorical):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfNodes()*TSize);
            for(const auto& r_node : mrModelPart.Nodes()){
                const array_1d<double, TSize>& r_val = r_node.GetValue(rVariable);
                for(int dim = 0 ; dim < TSize ; dim++){
                    data[counter++] = r_val[dim];
                }
            }
            break;
        }
        case (DataLocation::Element):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfElements()*TSize);
            for(const auto& r_node : mrModelPart.Elements()){
                const array_1d<double, TSize>& r_val = r_node.GetValue(rVariable);
                for(int dim = 0 ; dim < TSize ; dim++){
                    data[counter++] = r_val[dim];
                }
            }
            break;
        }
        case (DataLocation::Condition):{
            IndexType counter = 0;
            data.resize(mrModelPart.NumberOfConditions()*TSize);
            for(const auto& r_node : mrModelPart.Conditions()){
                const array_1d<double, TSize>& r_val = r_node.GetValue(rVariable);
                for(int dim = 0 ; dim < TSize ; dim++){
                    data[counter++] = r_val[dim];
                }
            }
            break;
        }
        case (DataLocation::ModelPart):{
            IndexType counter = 0;
            data.resize(TSize);
            auto& r_val = mrModelPart[rVariable];
            for(int dim = 0 ; dim < TSize ; dim++){
                    data[counter++] = r_val[dim];
                }
            break;
        }
        case (DataLocation::ProcessInfo):{
            IndexType counter = 0;
            data.resize(TSize);
            auto& r_val = mrModelPart.GetProcessInfo()[rVariable];
            for(int dim = 0 ; dim < TSize ; dim++){
                    data[counter++] = r_val[dim];
                }
            break;
        }
        default:{
            //Throw an error about invalid DataLocation
            KRATOS_ERROR << "unknown Datalocation" << std::endl;
            break;
        }

        }
        return data;
    }

    /// To Import a Scalar data (Double/int/...)
    template<typename TDataType>
    void SetScalarData(
        const Variable<TDataType>& rVariable,
        const DataLocation DataLoc,
        const std::vector<TDataType>& rData)
    {
        switch (DataLoc)
        {
        case (DataLocation::NodeHistorical):{
            ImportDataSizeCheck(mrModelPart.NumberOfNodes(), rData.size());
            IndexType counter = 0;
            for(auto& r_node : mrModelPart.Nodes()){
                auto& r_val = r_node.FastGetSolutionStepValue(rVariable);
                r_val = rData[counter++];
            }
            break;
        }
        case (DataLocation::NodeNonHistorical):{
            ImportDataSizeCheck(mrModelPart.NumberOfNodes(), rData.size());
            SetScalarDataFromContainer(mrModelPart.Nodes(), rVariable, rData);
            break;
        }
        case (DataLocation::Element):{
            ImportDataSizeCheck(mrModelPart.NumberOfElements(), rData.size());
            SetScalarDataFromContainer(mrModelPart.Elements(), rVariable, rData);
            break;
        }
        case (DataLocation::Condition):{
            ImportDataSizeCheck(mrModelPart.NumberOfConditions(), rData.size());
            SetScalarDataFromContainer(mrModelPart.Conditions(), rVariable, rData);
            break;
        }
        case (DataLocation::ModelPart):{
            mrModelPart[rVariable]= rData[0];
            break;
        }
        case (DataLocation::ProcessInfo):{
            mrModelPart.GetProcessInfo()[rVariable] = rData[0] ;
            break;
        }
        default:{
            KRATOS_ERROR << "unknown Datalocation" << std::endl;
            break;
        }
        }

    }

    /// To Import a Vector data
    template<class TDataType>
    void SetVectorData(
        const Variable<TDataType>& rVariable,
        const DataLocation DataLoc,
        const std::vector<double>& rData)
    {
        switch (DataLoc)
        {
        case (DataLocation::NodeHistorical):{
            const std::size_t size = mrModelPart.NumberOfNodes() > 0 ? mrModelPart.NodesBegin()->FastGetSolutionStepValue(rVariable).size() : 0;

            IndexType counter = 0;
            for(auto& r_node : mrModelPart.Nodes()){
                auto& r_val = r_node.FastGetSolutionStepValue(rVariable);

                KRATOS_DEBUG_ERROR_IF(r_val.size() != size) << "mismatch in size!" << std::endl;

                for(int dim = 0 ; dim < size ; dim++){
                    r_val[dim] = rData[counter++];
                }
            }
            break;
        }
        case (DataLocation::NodeNonHistorical):{
            const std::size_t size = mrModelPart.NumberOfNodes() > 0 ? mrModelPart.NodesBegin()->GetValue(rVariable).size() : 0;
            SetVectorDataFromContainer(mrModelPart.Nodes(), size, rVariable, rData);
            break;
        }
        case (DataLocation::Element):{
            const std::size_t size = mrModelPart.NumberOfElements() > 0 ? mrModelPart.ElementsBegin()->GetValue(rVariable).size() : 0;
            SetVectorDataFromContainer(mrModelPart.Elements(), size, rVariable, rData);
            break;
        }
        case (DataLocation::Condition):{
            const std::size_t size = mrModelPart.NumberOfConditions() > 0 ? mrModelPart.ConditionsBegin()->GetValue(rVariable).size() : 0;
            SetVectorDataFromContainer(mrModelPart.Conditions(), size, rVariable, rData);
            break;
        }
        case (DataLocation::ModelPart):{
            const std::size_t size = mrModelPart[rVariable].size();
            IndexType counter = 0;
            auto& r_val = mrModelPart[rVariable];
                for(int dim = 0 ; dim < size ; dim++){
                    r_val[dim] = rData[counter++];
                }
            break;
            }
        case (DataLocation::ProcessInfo):{
            const std::size_t size = mrModelPart.GetProcessInfo()[rVariable].size();
            IndexType counter = 0;
            auto& r_val = mrModelPart.GetProcessInfo()[rVariable];
            for(int dim = 0 ; dim < size ; dim++){
                    r_val[dim] = rData[counter++];
                }
            break;
        }
        default:{
            KRATOS_ERROR << "unknown Datalocation" << std::endl;
            break;
        }

        }

    }


    /// Turn back information as a string.
    virtual std::string Info() const
    {
        return "AuxiliarModelPartUtilities";
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << Info() << std::endl;
    }

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
        rOStream << Info() << std::endl;
    }

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
    ///@name Static Member Variables
    ///@{
    ///@}
    ///@name Member Variables
    ///@{

    ModelPart& mrModelPart;

    ///@}
    ///@name Private Operators
    ///@{

    ///@}
    ///@name Private Operations
    ///@{

    template<typename TDataType, class TContainerType>
    void SetScalarDataFromContainer(TContainerType& rContainer, const Variable<TDataType>& rVariable, const std::vector<TDataType>& rData)
    {
        IndexType counter = 0;
        for(auto& r_entity : rContainer){
            auto& r_val = r_entity.GetValue(rVariable);
            r_val = rData[counter++];
        }

        /* IndexPartition<std::size_t>(rContainer.size()).for_each([&](std::size_t index){
            rContainer[index].GetValue(rVariable) = rData[index];
        }); */
                
        /* BlockPartition<TContainerType>(rContainer).for_each([](TContainerType::value_type& rEntity){

        })

        LockObject my_lock;

        block_for_each(mrModelPart.Conditions(), [](Condition& rCond){
            for (auto& r_node : rCond.Points()) {
                my_lock.SetLock();
                r_node.SetLock();
                r_node.SetValue(DISPLACEMENT_X) += rCond.Id();
                r_node.UnSetLock();
                my_lock.UnSetLock();
            }
        }) */
    }

    template<typename TDataType, class TContainerType>
    void SetVectorDataFromContainer(TContainerType& rContainer, const std::size_t size, const Variable<TDataType>& rVariable, const std::vector<double>& rData)
    {
        IndexType counter = 0;
        for(auto& r_entity : rContainer){
            auto& r_val = r_entity.GetValue(rVariable);
            KRATOS_DEBUG_ERROR_IF(r_val.size() != size) << "mismatch in size!" << std::endl;
            for(int dim = 0 ; dim < size ; dim++){
                r_val[dim] = rData[counter++];
            }
        }
    }

    //template<class TContainerType>
    void ImportDataSizeCheck(int rContainer_size, int rSize){
        KRATOS_DEBUG_ERROR_IF(rContainer_size != rSize) << "mismatch in size!" << std::endl;
    } 


    ///@}
    ///@name Private  Access
    ///@{
    ///@}

    ///@}
    ///@name Serialization
    ///@{

    friend class Serializer;

    void save(Serializer& rSerializer) const
    {
    }

    void load(Serializer& rSerializer)
    {
    }

    ///@name Private Inquiry
    ///@{
    ///@}

    ///@name Unaccessible methods
    ///@{
    ///@}
};// class AuxiliarModelPartUtilities

///@}
///@name Type Definitions
///@{


///@}
///@name Input and output
///@{

///@}

}  // namespace Kratos.
#endif /* KRATOS_AUXILIAR_MODEL_PART_UTILITIES defined */
