/*
==============================================================================
KratosStructuralApplication
A library based on:
Kratos
A General Purpose Software for Multi-Physics Finite Element Analysis
Version 1.0 (Released on march 05, 2007).

Copyright 2007
Pooyan Dadvand, Riccardo Rossi, Janosch Stascheit, Felix Nagel
pooyan@cimne.upc.edu
rrossi@cimne.upc.edu
janosch.stascheit@rub.de
nagel@sd.rub.de
- CIMNE (International Center for Numerical Methods in Engineering),
Gran Capita' s/n, 08034 Barcelona, Spain
- Ruhr-University Bochum, Institute for Structural Mechanics, Germany


Permission is hereby granted, free  of charge, to any person obtaining
a  copy  of this  software  and  associated  documentation files  (the
"Software"), to  deal in  the Software without  restriction, including
without limitation  the rights to  use, copy, modify,  merge, publish,
distribute,  sublicense and/or  sell copies  of the  Software,  and to
permit persons to whom the Software  is furnished to do so, subject to
the following condition:

Distribution of this code for  any  commercial purpose  is permissible
ONLY BY DIRECT ARRANGEMENT WITH THE COPYRIGHT OWNERS.

The  above  copyright  notice  and  this permission  notice  shall  be
included in all copies or substantial portions of the Software.

THE  SOFTWARE IS  PROVIDED  "AS  IS", WITHOUT  WARRANTY  OF ANY  KIND,
EXPRESS OR  IMPLIED, INCLUDING  BUT NOT LIMITED  TO THE  WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT  SHALL THE AUTHORS OR COPYRIGHT HOLDERS  BE LIABLE FOR ANY
CLAIM, DAMAGES OR  OTHER LIABILITY, WHETHER IN AN  ACTION OF CONTRACT,
TORT  OR OTHERWISE, ARISING  FROM, OUT  OF OR  IN CONNECTION  WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

==============================================================================
*/
//
//   Project Name:        Kratos
//   Last Modified by:    $Author: nagel $
//   Date:                $Date: 2009-03-20 08:55:34 $
//   Revision:            $Revision: 1.20 $
//
//


#if !defined(KRATOS_STRUCTURAL_APPLICATION_H_INCLUDED )
#define  KRATOS_STRUCTURAL_APPLICATION_H_INCLUDED



// System includes
#include <string>
#include <iostream>


// External includes


// Project includes
#include "includes/define.h"
#include "includes/serializer.h"

#include "includes/kratos_application.h"
#include "includes/constitutive_law.h"

#include "custom_elements/total_lagrangian.h"
//#include "custom_elements/linear_incompresible_element.h"
#include "custom_elements/mixed_lagrangian.h"
#include "custom_elements/beam_element.h"
#include "custom_elements/timoshenko_beam_element.h"
#include "custom_elements/kinematic_linear.h"
#include "custom_elements/membrane_element.h"
#include "custom_elements/unsaturated_soils_element_2phase_small_strain.h"
#include "custom_elements/unsaturated_soils_element_3phase_small_strain.h"
// #include "custom_elements/upc_test_element.h"
#include "custom_elements/shell_isotropic.h"
#include "custom_elements/shell_anisotropic.h"
#include "custom_elements/shell_anisotropic_linear.h"
#include "custom_elements/crisfield_truss_element.h"
#include "custom_elements/ebst.h"
#include "custom_elements/ebst_vel.h"
#include "custom_elements/eas_element_q4e4.h"

#include "custom_conditions/pointforce3D.h"
#include "custom_conditions/node_tying_lagrange.h"
#include "custom_conditions/node_tying_lagrange_z.h"
#include "custom_conditions/face2D.h"
#include "custom_conditions/face3D.h"
#include "custom_conditions/face_pressure3D.h"
#include "custom_conditions/faceforce3D.h"
#include "custom_conditions/line_force.h"
#include "custom_conditions/contact_link_3D.h"
#include "custom_conditions/contact_link_3D_newmark.h"
#include "custom_conditions/master_contact_face_3D.h"
#include "custom_conditions/master_contact_face_3D_newmark.h"
#include "custom_conditions/slave_contact_face_3D.h"
#include "custom_conditions/slave_contact_face_3D_newmark.h"
#include "custom_conditions/pointforce3D.h"
#include "custom_conditions/pointforce2D.h"
#include "custom_conditions/pointmoment3D.h"
#include "custom_conditions/master_contact_face_2d.h"
#include "custom_conditions/slave_contact_point_2d.h"
#include "custom_conditions/face_vel_3D.h"
#include "custom_conditions/point_point_joint_condition.h"
#include "custom_conditions/elastic_constraint.h"

#include "constitutive_laws/isotropic_2d.h"
#include "constitutive_laws/isotropic_3d.h"
#include "constitutive_laws/dummy_constitutive_law.h"
#include "constitutive_laws/drucker_prager.h"


#include "includes/variables.h"
#include "includes/deprecated_variables.h"
#include "includes/legacy_structural_app_vars.h"
#include "includes/ublas_interface.h"




namespace Kratos
{
///@name Kratos Globals
///@{
// Variables definition
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, WRINKLING_APPROACH )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, GREEN_LAGRANGE_STRAIN_TENSOR )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, PK2_STRESS_TENSOR )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, AUXILIARY_MATRIX_1 )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, YOUNG_MODULUS )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, POISSON_RATIO )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, MU )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, ALPHA )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, RETRACTION_TIME )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, THICKNESS )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, NEGATIVE_FACE_PRESSURE )
//	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, POSITIVE_FACE_PRESSURE )

typedef Matrix fix_matrix_33;
//typedef bounded_matrix<double,3,3> fix_matrix_33;
typedef Vector array3;
//typedef array_1d<double,3> array3;

//     KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  ConstitutiveLaw::Pointer, CONSTITUTIVE_LAW );

KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  fix_matrix_33 , MATRIX_A )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  fix_matrix_33 , MATRIX_B )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  fix_matrix_33 , MATRIX_D )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  array3, COMPOSITE_DIRECTION )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  array3, ORTHOTROPIC_YOUNG_MODULUS )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  array3, ORTHOTROPIC_SHEAR_MODULUS )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Matrix, ORTHOTROPIC_POISSON_RATIO )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Matrix , GEOMETRIC_STIFFNESS )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Matrix , MATERIAL_DIRECTION )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Matrix , JOINT_STIFFNESS )
//CONTACT_LINK_MASTER is defined in condition.h
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Condition::Pointer, CONTACT_LINK_MASTER )
//CONTACT_LINK_SLAVE is defined in condition.h
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Condition::Pointer, CONTACT_LINK_SLAVE )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Node<3>::Pointer,  NEAR_NODE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Point, MASTER_CONTACT_LOCAL_POINT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Point, MASTER_CONTACT_CURRENT_LOCAL_POINT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Point, MASTER_CONTACT_LAST_CURRENT_LOCAL_POINT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Point, SLAVE_CONTACT_LOCAL_POINT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Point, MASTER_CONTACT_GLOBAL_POINT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Point, MASTER_CONTACT_CURRENT_GLOBAL_POINT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Point, SLAVE_CONTACT_GLOBAL_POINT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double , INSITU_STRESS_SCALE )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, REFERENCE_WATER_PRESSURE )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double , OVERCONSOLIDATION_RATIO )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, EXCESS_PORE_WATER_PRESSURE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, COORDINATES)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, FLUID_FLOWS)

KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double , CONTACT_PENETRATION )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, DAMAGE_E0 )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, DAMAGE_EF )

KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, BASE )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, HEIGHT )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, CROSS_AREA)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, AREA )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, AREA_X )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, AREA_Y )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, AREA_Z )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, INERTIA_X)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, INERTIA_Y)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, INERTIA_Z)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, FC)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, FT)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, CONCRETE_YOUNG_MODULUS_C)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, CONCRETE_YOUNG_MODULUS_T)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, FRACTURE_ENERGY)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, CRUSHING_ENERGY)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, ELASTIC_ENERGY)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, PLASTIC_ENERGY)

//       KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, YIELD_STRESS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, PLASTIC_MODULUS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, PLASTICITY_INDICATOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, ISOTROPIC_HARDENING_MODULUS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, KINEMATIC_HARDENING_MODULUS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, LAMNDA) // Load factor
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DAMAGE )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, ORTHOTROPIC_ANGLE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, VOLUMEN_FRACTION)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, MAX_INTERNAL_FRICTION_ANGLE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DILATANCY_ANGLE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, MAX_DILATANCY_ANGLE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, COHESION)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, ISOTROPIC_ELASTIC_LIMIT)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, ORTHOTROPIC_ELASTIC_LIMIT)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, VECTOR_DAMAGE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, ORTHOTROPIC_YOUNG_MODULUS_2D) // [E1 E2 G12]
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, ORTHOTROPIC_POISSON_RATIO_2D) // [v12 v21]
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, GREEN_LAGRANGE_PLASTIC_STRAIN_TENSOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, PLASTIC_STRAIN_VECTOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, CURRENT_STRAIN_VECTOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, EQUIVALENT_VOLUMETRIC_PLASTIC_STRAIN)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, EQUIVALENT_DEVIATORIC_PLASTIC_STRAIN)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, ALMANSI_PLASTIC_STRAIN)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, ALMANSI_ELASTIC_STRAIN)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, NODAL_STRESS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, NODAL_STRAIN)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, CONSTRAINT_MATRIX)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, PRESTRESS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, PRESTRESS_FACTOR )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, CONSTRAINT_VECTOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DISIPATION)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int,     NODAL_VALUES)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double,  NODAL_DAMAGE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, bool, IS_TARGET)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, bool, IS_CONTACTOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DAMPING_RATIO)
//KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, KINETIC_ENERGY)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, POTENCIAL_ENERGY)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DEFORMATION_ENERGY)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, VON_MISES_STRESS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, YIELD_SURFACE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, RHS_PRESSURE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, bool, COMPUTE_TANGENT_MATRIX)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, IS_DISCRETE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, TENSILE_STRENGTH)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, SHEAR_STRENGTH)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, VISCOUS_DAMPING)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, MAX_FRECUENCY1)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(JOINT_FORCE_REACTION)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(JOINT_MOMENT_REACTION)
//KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(INTERNAL_FORCE) //already put on variables.h (warning was appearing on Windows)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ELASTIC_BEDDING_STIFFNESS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, bool, IS_BBAR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, INTEGRATION_ORDER)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, NEIGHBOUR_EXPANSION_LEVEL)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, STRESS_RECOVERY_TYPE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, RAYLEIGH_DAMPING_ALPHA)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, RAYLEIGH_DAMPING_BETA)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, STABILISATION_FACTOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, SHEAR_MODULUS)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, RECOVERY_STRESSES)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, PRE_STRAIN_VECTOR)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, POST_STRAIN_VECTOR)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(PRESCRIBED_DELTA_DISPLACEMENT)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, bool,   IS_CONTACT_NODE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, LAGRANGE_MULTIPLIER)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, bool, HAS_STRAIN_AT_NODE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, bool, HAS_STRESSES_AT_NODE)
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  bool, HAS_NODAL_ERROR )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  bool, FORCE_EQUAL_ORDER_INTERPOLATION )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, NODAL_ERROR_1 )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, DUMMY_DOF )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, PRECONSOLIDATION_PRESSURE_MIN )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, PRECONSOLIDATION_PRESSURE_DEF )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, CSL_SLOPE )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, VIRGIN_COMPRESSION_INDEX )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, SWELL_INDEX )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, VOID_RATIO )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Vector, NEIGHBOUR_WEIGHTS )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, MATERIAL_DENSITY )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, MATERIAL_DENSITY_NEW )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, MATERIAL_DENSITY_FILTERED )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, ELEMENT_DC )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, ELEMENT_DC_FILTERED )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, ELEMENT_DV )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, ELEMENT_DV_FILTERED )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, YOUNG_MODULUS_0 )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, YOUNG_MODULUS_MIN )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, PENALIZATION_FACTOR )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, GEOMETRICAL_DOMAIN_SIZE )
KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, JACOBIAN_0 )


// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DP_EPSILON )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, INSITU_STRESS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DP_ALPHA1 )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, DP_K )
//KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double,TO_ERASE )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, CALCULATE_INSITU_STRESS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, CONTACT_RAMP )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, PENALTY )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, INITIAL_PENALTY )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, MAXIMUM_PENALTY )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, RAMP_CRITERION )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, RAMP_FACTOR )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, PENALTY_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, INITIAL_PENALTY_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, MAXIMUM_PENALTY_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, RAMP_CRITERION_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, RAMP_FACTOR_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, double, FRICTION_COEFFICIENT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, LAMBDAS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, LAMBDAS_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, GAPS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Vector, DELTA_LAMBDAS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, Matrix, DELTA_LAMBDAS_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, MAX_UZAWA_ITERATIONS)
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, CONTACT_SLAVE_INTEGRATION_POINT_INDEX )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Matrix, CONTACT_LINK_M )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  int, CONTACT_DOUBLE_CHECK )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  int, IS_CONTACT_MASTER )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  int, IS_CONTACT_SLAVE )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, K_CONTACT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, K_CONTACT_T )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Vector, STICK )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  int, FIRST_TIME_STEP )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  int, QUASI_STATIC_ANALYSIS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Vector, NORMAL_STRESS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Vector, TANGENTIAL_STRESS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, NORMAL_CONTACT_STRESS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, TANGENTIAL_CONTACT_STRESS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, CONTACT_STICK )

//
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_DT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_ACCELERATION )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_NULL )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_NULL_DT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_NULL_ACCELERATION )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_EINS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_EINS_DT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, WATER_PRESSURE_EINS_ACCELERATION )
//
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_DT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_ACCELERATION )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_NULL )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_NULL_DT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_NULL_ACCELERATION )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_EINS )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_EINS_DT )
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  double, AIR_PRESSURE_EINS_ACCELERATION )
//
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_OLD)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_DT)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_NULL)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_NULL_DT)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION_NULL)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_EINS)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(DISPLACEMENT_EINS_DT)
// 	KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(ACCELERATION_EINS)
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION,  Matrix, ELASTIC_LEFT_CAUCHY_GREEN_OLD )
//
// 	KRATOS_DEFINE_APPLICATION_VARIABLE( LEGACY_STRUCTURAL_APPLICATION, int, ACTIVATION_LEVEL)
//KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(VAUX);
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

/// Structural Application for KRATOS.
/**
 * This application features Elements, Conditions, Constitutive laws and Utilities
 * for structural analysis problems
 */
class KratosStructuralApplication : public KratosApplication
{
public:
    ///@name Type Definitions
    ///@{

    /// Pointer definition of KratosStructuralApplication
    KRATOS_CLASS_POINTER_DEFINITION(KratosStructuralApplication);
    ///@}
    ///@name Life Cycle
    ///@{
    /// Default constructor.
    KratosStructuralApplication();

    /// Destructor.
    virtual ~KratosStructuralApplication() {}

    ///@}
    ///@name Operators
    ///@{
    ///@}
    ///@name Operations
    ///@{
    /**
     * Registers the structural application in the KRATOS kernel
     */
    virtual void Register();

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
    virtual std::string Info() const
    {
        return "KratosStructuralApplication";
    }

    /// Print information about this object.
    virtual void PrintInfo(std::ostream& rOStream) const
    {
        rOStream << Info();
        PrintData(rOStream);
    }

    /// Print object's data.
    virtual void PrintData(std::ostream& rOStream) const
    {
        KRATOS_WATCH("in KratosStructuralApplication application");
        KRATOS_WATCH(KratosComponents<VariableData>::GetComponents().size() );
        rOStream << "Variables:" << std::endl;
        KratosComponents<VariableData>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Elements:" << std::endl;
        KratosComponents<Element>().PrintData(rOStream);
        rOStream << std::endl;
        rOStream << "Conditions:" << std::endl;
        KratosComponents<Condition>().PrintData(rOStream);
    }
    ///@}
    ///@name Friends
    ///@{
    ///@}

private:
    ///@name Member Variables
    ///@{

    const CrisfieldTrussElement mCrisfieldTrussElement3D2N;
    const CrisfieldTrussElement mCrisfieldTrussElement3D3N;
    const BeamElement mBeamElement3D2N;
    const BeamElement mBeamElement3D3N;
    const TimoshenkoBeamElement mTimoshenkoBeamElement3D2N;
    const TimoshenkoBeamElement mTimoshenkoBeamElement3D3N;
    const ShellIsotropic mIsoShellElement;
    const ShellAnisotropic mAnisoShellElement;
    const ShellAnisotropicLinear mAnisoLinearShellElement;
    const MembraneElement mMembraneElement;

    //const LinearIncompresibleElement mLinearIncompresibleElement2D3N;
    //const LinearIncompresibleElement mLinearIncompresibleElement3D4N;

    const TotalLagrangian mTotalLagrangian2D3N;
    const TotalLagrangian mTotalLagrangian2D4N;
    const TotalLagrangian mTotalLagrangian2D6N;
    const TotalLagrangian mTotalLagrangian2D8N;
    const TotalLagrangian mTotalLagrangian3D4N;
    const TotalLagrangian mTotalLagrangian3D10N;
    const TotalLagrangian mTotalLagrangian3D6N;
    const TotalLagrangian mTotalLagrangian3D15N;
    const TotalLagrangian mTotalLagrangian3D8N;
    const TotalLagrangian mTotalLagrangian3D20N;
    const TotalLagrangian mTotalLagrangian3D27N;
	
    const MixedLagrangian mMixedLagrangian2D3N;
    const MixedLagrangian mMixedLagrangian2D4N;
    const MixedLagrangian mMixedLagrangian2D6N;
    const MixedLagrangian mMixedLagrangian2D8N;
    const MixedLagrangian mMixedLagrangian3D4N;
    const MixedLagrangian mMixedLagrangian3D10N;
    const MixedLagrangian mMixedLagrangian3D6N;
    const MixedLagrangian mMixedLagrangian3D15N;
    const MixedLagrangian mMixedLagrangian3D8N;
    const MixedLagrangian mMixedLagrangian3D20N;
    const MixedLagrangian mMixedLagrangian3D27N;

    const KinematicLinear mKinematicLinear2D3N;
    const KinematicLinear mKinematicLinear2D4N;
    const KinematicLinear mKinematicLinear2D6N;
    const KinematicLinear mKinematicLinear2D8N;
    const KinematicLinear mKinematicLinear2D9N;
    const KinematicLinear mKinematicLinear3D4N;
    const KinematicLinear mKinematicLinear3D10N;
    const KinematicLinear mKinematicLinear3D8N;
    const KinematicLinear mKinematicLinear3D20N;
    const KinematicLinear mKinematicLinear3D27N;
    const KinematicLinear mKinematicLinear3D6N;
    const KinematicLinear mKinematicLinear3D15N;
    const UnsaturatedSoilsElement_2phase_SmallStrain mUnsaturatedSoilsElement2PhaseSmallStrain3D4N;
    const UnsaturatedSoilsElement_2phase_SmallStrain mUnsaturatedSoilsElement2PhaseSmallStrain3D8N;
    const UnsaturatedSoilsElement_2phase_SmallStrain mUnsaturatedSoilsElement2PhaseSmallStrain3D10N;
    const UnsaturatedSoilsElement_2phase_SmallStrain mUnsaturatedSoilsElement2PhaseSmallStrain3D15N;
    const UnsaturatedSoilsElement_2phase_SmallStrain mUnsaturatedSoilsElement2PhaseSmallStrain3D20N;
    const UnsaturatedSoilsElement_2phase_SmallStrain mUnsaturatedSoilsElement2PhaseSmallStrain3D27N;
    const UnsaturatedSoilsElement_3phase_SmallStrain mUnsaturatedSoilsElement3PhaseSmallStrain3D4N;
    const UnsaturatedSoilsElement_3phase_SmallStrain mUnsaturatedSoilsElement3PhaseSmallStrain3D8N;
    const UnsaturatedSoilsElement_3phase_SmallStrain mUnsaturatedSoilsElement3PhaseSmallStrain3D10N;
    const UnsaturatedSoilsElement_3phase_SmallStrain mUnsaturatedSoilsElement3PhaseSmallStrain3D15N;
    const UnsaturatedSoilsElement_3phase_SmallStrain mUnsaturatedSoilsElement3PhaseSmallStrain3D20N;
    const UnsaturatedSoilsElement_3phase_SmallStrain mUnsaturatedSoilsElement3PhaseSmallStrain3D27N;
    const Ebst mEbst3D3N;
    const EbstVel mEbstVel3D3N;
    const EASElementQ4E4 mEASElementQ4E4;
    const Face2D  mFace2D;
    const Face3D  mFace3D3N;
    const Face3D  mFace3D6N;
    const Face3D  mFace3D4N;
    const Face3D  mFace3D8N;
    const Face3D  mFace3D9N;
    const FacePressure3D  mFacePressure3D3N;
    const FacePressure3D  mFacePressure3D6N;
    const FacePressure3D  mFacePressure3D4N;
    const FacePressure3D  mFacePressure3D8N;
    const FacePressure3D  mFacePressure3D9N;
    const LineForce mLineForce2D2N;
    const LineForce mLineForce2D3N;
    const LineForce mLineForce3D2N;
    const LineForce mLineForce3D3N;
    const FaceForce3D mFaceForce3D3N;
    const FaceForce3D mFaceForce3D6N;
    const FaceForce3D mFaceForce3D4N;
    const FaceForce3D mFaceForce3D8N;
    const FaceForce3D mFaceForce3D9N;
    const MasterContactFace3D mMasterContactFace3D;
    const MasterContactFace3D mMasterContactFace3D3;
    const MasterContactFace3D mMasterContactFace3D6;
    const MasterContactFace3D mMasterContactFace3D8;
    const MasterContactFace3D mMasterContactFace3D9;
    const SlaveContactFace3D mSlaveContactFace3D;
    const SlaveContactFace3D mSlaveContactFace3D3;
    const SlaveContactFace3D mSlaveContactFace3D6;
    const SlaveContactFace3D mSlaveContactFace3D8;
    const SlaveContactFace3D mSlaveContactFace3D9;
    const MasterContactFace3DNewmark mMasterContactFace3DNewmark;
    const MasterContactFace3DNewmark mMasterContactFace3D3Newmark;
    const MasterContactFace3DNewmark mMasterContactFace3D6Newmark;
    const MasterContactFace3DNewmark mMasterContactFace3D8Newmark;
    const MasterContactFace3DNewmark mMasterContactFace3D9Newmark;
    const SlaveContactFace3DNewmark mSlaveContactFace3DNewmark;
    const SlaveContactFace3DNewmark mSlaveContactFace3D3Newmark;
    const SlaveContactFace3DNewmark mSlaveContactFace3D6Newmark;
    const SlaveContactFace3DNewmark mSlaveContactFace3D8Newmark;
    const SlaveContactFace3DNewmark mSlaveContactFace3D9Newmark;
    const FaceVel3D  mFaceVel3D3N;
    const PointForce3D  mPointForce3D;
    const PointForce2D  mPointForce2D;
    const PointMoment3D mPointMoment3D;
    const ElasticConstraint mElasticPointConstraint;
    const ElasticConstraint mElasticLineConstraint2N;
    const ElasticConstraint mElasticLineConstraint3N;
    const ElasticConstraint mElasticFaceConstraint3N;
    const ElasticConstraint mElasticFaceConstraint6N;
    const ElasticConstraint mElasticFaceConstraint4N;
    const ElasticConstraint mElasticFaceConstraint8N;
    const ElasticConstraint mElasticFaceConstraint9N;
    const NodeTyingLagrange mNodeTyingLagrange;
    const NodeTyingLagrangeZ mNodeTyingLagrangeZ;
    const PointPointJointCondition mPointPointJointCondition;

    const SlaveContactPoint2D mSlaveContactPoint2D;
    const MasterContactFace2D mMasterContactFace2D;

    const Isotropic3D mIsotropic3D;
    const DummyConstitutiveLaw mDummyConstitutiveLaw;
    const DruckerPrager mDruckerPrager;


//             const UPCTestElement mUPCTestElement3D20N;
    ///@}
    ///@name Private Operators
    ///@{
    ///@}
    ///@name Private Operations
    ///@{
    ///@}
    ///@name Private  Access
    ///@{
    ///@}
    ///@name Private Inquiry
    ///@{
    ///@}
    ///@name Un accessible methods
    ///@{

    /// Assignment operator.
    KratosStructuralApplication& operator=(KratosStructuralApplication const& rOther);

    /// Copy constructor.
    KratosStructuralApplication(KratosStructuralApplication const& rOther);

    ///@}
}; // Class KratosStructuralApplication
///@}
}  // namespace Kratos.
#endif // KRATOS_STRUCTURAL_APPLICATION_H_INCLUDED  defined 
