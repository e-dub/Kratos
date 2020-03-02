//    |  /           |
//    ' /   __| _` | __|  _ \   __|
//    . \  |   (   | |   (   |\__ `
//   _|\_\_|  \__,_|\__|\___/ ____/
//                   Multi-Physics
//
//  License:		 BSD License
//					 Kratos default license: kratos/license.txt
//
//  Main authors:    Anoop Kodakkal, Mate Pentek
//
//

// System includes

// External includes

// Project includes
#include "geometries/geometry.h"
#include "geometries/geometry_data.h"
#include "utilities/openmp_utils.h"
#include "utilities/variable_utils.h"

// Application includes
#include "drag_and_moment_utilities.h"
#include "fluid_dynamics_application_variables.h"

namespace Kratos
{
    /* Public functions *******************************************************/

    array_1d<double, 6> DragAndMomentUtilities::CalculateBodyFittedDragAndMoment(ModelPart& rModelPart, array_1d<double, 3> rReferencePoint) {

        array_1d<double, 6> drag_force_moment = ZeroVector(6);
        double dx = 0.0;
        double dy = 0.0;
        double dz = 0.0;
        double fx = 0.0;
        double fy = 0.0;
        double fz = 0.0;

        #pragma omp parallel for reduction(+:dx,dy,dz,fx,fy,fz)
        for(int i_node = 0; i_node < static_cast<int>(rModelPart.NumberOfNodes()); i_node++){
            auto it_node = rModelPart.NodesBegin() + i_node;
            auto drag = it_node->GetSolutionStepValue(REACTION,0);
            auto x = it_node->X() - rReferencePoint[0];
            auto y = it_node->Y() - rReferencePoint[1];
            auto z = it_node->Z() - rReferencePoint[2];
            dx += -1 * drag[0];
            dy += -1 * drag[1];
            dz += -1 * drag[2];
            fx +=  y * (-1) * drag[2] - z * (-1) * drag[1];
            fy +=  z * (-1) * drag[0] - x * (-1) * drag[2];
            fz +=  x * (-1) * drag[1] - y * (-1) * drag[0];
        }
        // three drag components
        drag_force_moment[0] = dx;
        drag_force_moment[1] = dy;
        drag_force_moment[2] = dz;
        // three base moment components
        drag_force_moment[3] = fx;
        drag_force_moment[4] = fy;
        drag_force_moment[5] = fz;

        // Perform MPI synchronization
        //drag_force_moment = rModelPart.GetCommunicator().GetDataCommunicator().SumAll(drag_force_moment);
        return drag_force_moment;
    }

    array_1d<double, 6> DragAndMomentUtilities::CalculateEmbeddedDragAndMoment(ModelPart& rModelPart, array_1d<double, 3> rReferencePoint) {

        array_1d<double, 6> drag_force_moment = ZeroVector(6);
        double dx = 0.0;
        double dy = 0.0;
        double dz = 0.0;
        double fx = 0.0;
        double fy = 0.0;
        double fz = 0.0;

        // Iterate the model part elements to compute the drag
        array_1d<double, 3> elem_drag;
        array_1d<double, 3> elem_drag_center;

        #pragma omp parallel for reduction(+:dx,dy,dz,fx,fy,fz)
        for(int i = 0; i < static_cast<int>(rModelPart.Elements().size()); ++i){
            auto it_elem = rModelPart.ElementsBegin() + i;
            it_elem->Calculate(DRAG_FORCE_CENTER, elem_drag_center, rModelPart.GetProcessInfo());
            it_elem->Calculate(DRAG_FORCE, elem_drag, rModelPart.GetProcessInfo());
            dx += elem_drag[0];
            dy += elem_drag[1];
            dz += elem_drag[2];
            fx +=  (elem_drag_center[1] - rReferencePoint[1]) * elem_drag[2] - (elem_drag_center[2] - rReferencePoint[2]) * elem_drag[1];;
            fy +=  (elem_drag_center[2] - rReferencePoint[2]) * elem_drag[0] - (elem_drag_center[0] - rReferencePoint[0]) * elem_drag[2];
            fz +=  (elem_drag_center[0] - rReferencePoint[0]) * elem_drag[1] - (elem_drag_center[1] - rReferencePoint[1]) * elem_drag[0];
            
        }

        // three drag components
        drag_force_moment[0] = dx;
        drag_force_moment[1] = dy;
        drag_force_moment[2] = dz;
        // three base moment components
        drag_force_moment[3] = fx;
        drag_force_moment[4] = fy;
        drag_force_moment[5] = fz;
        

        // Perform MPI synchronization
        //drag_force_moment = rModelPart.GetCommunicator().GetDataCommunicator().SumAll(drag_force_moment);
        return drag_force_moment;
    }


    /* External functions *****************************************************/

    /// output stream function
    inline std::ostream& operator << (
        std::ostream& rOStream,
        const DragAndMomentUtilities& rThis) {

        rThis.PrintData(rOStream);
        return rOStream;
    }

}
