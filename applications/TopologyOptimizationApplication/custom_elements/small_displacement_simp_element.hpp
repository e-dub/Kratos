// ==============================================================================
//  KratosTopologyOptimizationApplication
//
//  License:         BSD License
//                   license: TopologyOptimizationApplication/license.txt
//
//  Main authors:    Baumgärtner Daniel, https://github.com/dbaumgaertner
//                   Octaviano Malfavón Farías
//                   Eric Gonzales
//
// ==============================================================================

#if !defined(KRATOS_SMALL_DISPLACEMENT_SIMP_ELEMENT_H_INCLUDED )
#define  KRATOS_SMALL_DISPLACEMENT_SIMP_ELEMENT_H_INCLUDED

// Project includes
///#include <boost/numeric/ublas/storage.hpp> 

#include "structural_mechanics_application.h"
#include "custom_elements/small_displacement.h"
#include "includes/define.h"
#include "custom_elements/base_solid_element.h"
#include "includes/variables.h"


namespace Kratos
{
///@name Kratos Globals
///@{
///@}
///@name Type Definitions
///@{
///@}
///@name  Enum's
///@{
///@}
///@name  Functions
///@{
///@}
///@name Kratos Classes
///@{

/// Topology Optimization Small Displacement Element for 3D geometries.

/**
 * Implements a Topology Optimization Small Displacement Lagrangian definition for structural analysis.
 * This works for arbitrary geometries in 3D
 */
class KRATOS_API(TOPOLOGY_OPTIMIZATION_APPLICATION) SmallDisplacementSIMPElement
    : public SmallDisplacement
{
public:

    ///@name Type Definitions
    ///@{
    ///Reference type definition for constitutive laws
    typedef ConstitutiveLaw ConstitutiveLawType;
    ///Pointer type for constitutive laws
    typedef ConstitutiveLawType::Pointer ConstitutiveLawPointerType;
    ///StressMeasure from constitutive laws
    typedef ConstitutiveLawType::StressMeasure StressMeasureType;
    ///Type definition for integration methods
    typedef GeometryData::IntegrationMethod IntegrationMethod;
    ///Type for element variables
    ///typedef SmallDisplacement::ElementDataType ElementDataType;


    /// hinugefügt am 10.02.
    /// The base element type
    typedef SmallDisplacement BaseType;

    /// The definition of the index type
    typedef std::size_t IndexType;

    /// The definition of the sizetype
    typedef std::size_t SizeType;
    /// bis Hier!!!!!!!

    /// Counted pointer of SmallDisplacementSIMPElement
    KRATOS_CLASS_POINTER_DEFINITION( SmallDisplacementSIMPElement );
    
    ///@}
    ///@name Life Cycle
    ///@{

    /// Default constructors
    SmallDisplacementSIMPElement(IndexType NewId, GeometryType::Pointer pGeometry);

    SmallDisplacementSIMPElement(IndexType NewId, GeometryType::Pointer pGeometry, PropertiesType::Pointer pProperties);

    ///Copy constructor
    SmallDisplacementSIMPElement(SmallDisplacementSIMPElement const& rOther);

    /// Destructor.
    ~SmallDisplacementSIMPElement() override;

    ///@}
    ///@name Operators
    ///@{

    /// Assignment operator.
    SmallDisplacementSIMPElement& operator=(SmallDisplacementSIMPElement const& rOther);

    ///@}
    ///@name Operations
    ///@{
    /**
     * Returns the currently selected integration method
     * @return current integration method selected
     */
    /**
     * creates a new total lagrangian updated element pointer
     * @param NewId: the ID of the new element
     * @param ThisNodes: the nodes of the new element
     * @param pProperties: the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Create(IndexType NewId, NodesArrayType const& ThisNodes, PropertiesType::Pointer pProperties) const;

    Element::Pointer Create(
    IndexType NewId,
    GeometryType::Pointer pGeom,
    PropertiesType::Pointer pProperties
    ) const override;

    /**
     * clones the selected element variables, creating a new one
     * @param NewId: the ID of the new element
     * @param ThisNodes: the nodes of the new element
     * @param pProperties: the properties assigned to the new element
     * @return a Pointer to the new element
     */
    Element::Pointer Clone(IndexType NewId, NodesArrayType const& ThisNodes) const;


    // =============================================================================================================================================
    // STARTING / ENDING METHODS
    // =============================================================================================================================================

    /// Function that gets the value on the Integration Point (For printing purposes in the output GiD)
    void GetValueOnIntegrationPoints(const Variable<double>& rVariable, std::vector<double>& rValues, const ProcessInfo& rCurrentProcessInfo) override;

    /// Function to calculate the sensitivities and the objective function
    void Calculate(const Variable<double>& rVariable, double& rOutput, const ProcessInfo& rCurrentProcessInfo) override;

    /// Function that overwrites the CalculateOnIntegrationPoints, to insert the X_PHYS into all Gauss Points of the given element
    /// That allows printing X_PHYS as elemental value in GiD
    void CalculateOnIntegrationPoints(const Variable<double>& rVariable, std::vector<double>& rOutput, const ProcessInfo& rCurrentProcessInfo) override;

    // =============================================================================================================================================
    // =============================================================================================================================================


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

protected:
    ///@name Protected static Member Variables
    ///@{
    ///@}
    ///@name Protected member Variables
    ///@{

    ///@}
    ///@name Protected Operators
    ///@{

    SmallDisplacementSIMPElement() : SmallDisplacement()
    {
    }


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


    ///@}
    ///@name Private Operators
    ///@{


    ///@}
    ///@name Private Operations
    ///@{
    template<class TType>
        void GetValueOnConstitutiveLaw(
            const Variable<TType>& rVariable,
            std::vector<TType>& rOutput
            )
        {
            const GeometryType::IntegrationPointsArrayType& integration_points = GetGeometry().IntegrationPoints( this->GetIntegrationMethod() );

            for ( IndexType point_number = 0; point_number <integration_points.size(); ++point_number ) {
                mConstitutiveLawVector[point_number]->GetValue( rVariable,rOutput[point_number]);
            }
        }

    ///@}
    ///@name Private  Access
    ///@{
    ///@}

    ///@}
    ///@name Serialization
    ///@{
    friend class Serializer;

    // A private default constructor necessary for serialization
    virtual void save(Serializer& rSerializer) const;
    virtual void load(Serializer& rSerializer);


    ///@name Private Inquiry
    ///@{
    ///@}
    ///@name Un accessible methods
    ///@{
    ///@}

}; // Class SmallDisplacementSIMPElement

///@}
///@name Type Definitions
///@{
///@}
///@name Input and output
///@{
///@}

} // namespace Kratos.
#endif // KRATOS_SMALL_DISPLACEMENT_TOPLOGY_OPTIMIZATION_ELEMENT_H_INCLUDED  defined
