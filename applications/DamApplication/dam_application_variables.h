//
//   Project Name:        KratosDamApplication $
//   Last Modified by:    $Author:     LGracia $
//   Date:                $Date:    March 2016 $
//   Revision:            $Revision:       1.0 $
//

#if !defined(KRATOS_DAM_APPLICATION_VARIABLES_H_INCLUDED )
#define  KRATOS_DAM_APPLICATION_VARIABLES_H_INCLUDED

// External includes
#include "includes/define.h"
#include "includes/kratos_application.h"
#include "includes/variables.h"
#include "structural_mechanics_application_variables.h"
#include "poromechanics_application_variables.h"


namespace Kratos
{
    typedef array_1d<double,3> Vector3;

    //Define Variables
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, TIME_UNIT_CONVERTER )

    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, THERMAL_EXPANSION )

    // Thermal Variables
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Matrix, THERMAL_STRESS_TENSOR )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Matrix, MECHANICAL_STRESS_TENSOR )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Matrix, THERMAL_STRAIN_TENSOR )

    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Vector, THERMAL_STRESS_VECTOR )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Vector, MECHANICAL_STRESS_VECTOR )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Vector, THERMAL_STRAIN_VECTOR )

    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, ALPHA_HEAT_SOURCE )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, TIME_ACTIVATION )

    // Output Variables
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Vector3, Vi_POSITIVE )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Vector3, Viii_POSITIVE )

    // Wave Equation
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, Dt_PRESSURE )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, Dt2_PRESSURE )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, VELOCITY_PRESSURE_COEFFICIENT )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, ACCELERATION_PRESSURE_COEFFICIENT )

    // Others
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, NODAL_YOUNG_MODULUS )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, ADDED_MASS )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, NODAL_REFERENCE_TEMPERATURE )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Matrix, NODAL_CAUCHY_STRESS_TENSOR )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, Matrix, INITIAL_NODAL_CAUCHY_STRESS_TENSOR )
    KRATOS_DEFINE_APPLICATION_VARIABLE(DAM_APPLICATION, double, PLACEMENT_TEMPERATURE )

    // From Solid
    KRATOS_DEFINE_3D_APPLICATION_VARIABLE_WITH_COMPONENTS( DAM_APPLICATION, FORCE_LOAD )
    KRATOS_DEFINE_APPLICATION_VARIABLE( DAM_APPLICATION, bool, COMPUTE_CONSISTENT_MASS_MATRIX )

    // Joints
    KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, MAX_COMPRESSIVE_STRESS )
    KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, MAX_TENSILE_STRESS )
    KRATOS_DEFINE_APPLICATION_VARIABLE( POROMECHANICS_APPLICATION, double, COHESION )

}  // namespace Kratos.

#endif /* KRATOS_DAM_APPLICATION_VARIABLES_H_INCLUDED  */
