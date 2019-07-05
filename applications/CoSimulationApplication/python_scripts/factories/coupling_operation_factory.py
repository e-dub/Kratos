from __future__ import print_function, absolute_import, division  # makes these scripts backward compatible with python 2.6 and 2.7

from . import base_factory

def CreateCouplingOperation(coupling_operation_settings, solvers):
    """This function creates and returns the Coupling Operation used for CoSimulation"""
    return base_factory.Create(convergence_accelerator_settings, [solvers], "KratosMultiphysics.CoSimulationApplication.coupling_operations")
