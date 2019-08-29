//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Suneth Warnakulasuriya (https://github.com/sunethwarna)
//

#if !defined(KRATOS_RANS_MODELLING_APPLICATION_VARIABLES_H_INCLUDED)
#define KRATOS_RANS_MODELLING_APPLICATION_VARIABLES_H_INCLUDED

// System includes
#include <vector>

// External includes

// Project includes
#include "includes/define.h"
#include "includes/kratos_application.h"
#include "includes/variables.h"
#include "processes/process.h"

namespace Kratos
{
KRATOS_DEFINE_VARIABLE(double, TURBULENT_KINETIC_ENERGY)
KRATOS_DEFINE_VARIABLE(double, TURBULENT_ENERGY_DISSIPATION_RATE)
KRATOS_DEFINE_VARIABLE(double, TURBULENT_KINETIC_ENERGY_RATE)
KRATOS_DEFINE_VARIABLE(double, TURBULENT_ENERGY_DISSIPATION_RATE_2)
KRATOS_DEFINE_VARIABLE(bool, IS_CO_SOLVING_PROCESS_ACTIVE)
KRATOS_DEFINE_VARIABLE(double, RANS_Y_PLUS)
KRATOS_DEFINE_VARIABLE(double, RANS_WALL_Y_PLUS)
KRATOS_DEFINE_VARIABLE(double, RANS_AUXILIARY_VARIABLE_1)
KRATOS_DEFINE_VARIABLE(double, RANS_AUXILIARY_VARIABLE_2)
KRATOS_DEFINE_VARIABLE(double, WALL_SMOOTHNESS_BETA)
KRATOS_DEFINE_VARIABLE(double, WALL_VON_KARMAN)
KRATOS_DEFINE_3D_VARIABLE_WITH_COMPONENTS(WALL_VELOCITY)
KRATOS_DEFINE_VARIABLE(double, TURBULENCE_RANS_C_MU)
KRATOS_DEFINE_VARIABLE(double, TURBULENCE_RANS_C1)
KRATOS_DEFINE_VARIABLE(double, TURBULENCE_RANS_C2)
KRATOS_DEFINE_VARIABLE(double, TURBULENT_VISCOSITY_MIN)
KRATOS_DEFINE_VARIABLE(double, TURBULENT_VISCOSITY_MAX)
KRATOS_DEFINE_VARIABLE(double, TURBULENT_KINETIC_ENERGY_SIGMA)
KRATOS_DEFINE_VARIABLE(double, TURBULENT_ENERGY_DISSIPATION_RATE_SIGMA)

KRATOS_DEFINE_VARIABLE(std::vector<Process::Pointer>, RANS_AUXILIARY_PROCESS_LIST)
KRATOS_DEFINE_VARIABLE(Element::WeakPointer, PARENT_ELEMENT)
KRATOS_DEFINE_VARIABLE(int, NUMBER_OF_NEIGHBOUR_CONDITIONS)


// Adjoint variables
KRATOS_DEFINE_VARIABLE( Matrix, RANS_Y_PLUS_VELOCITY_DERIVATIVES )

} // namespace Kratos

#endif /* KRATOS_RANS_MODELLING_APPLICATION_VARIABLES_H_INCLUDED */
