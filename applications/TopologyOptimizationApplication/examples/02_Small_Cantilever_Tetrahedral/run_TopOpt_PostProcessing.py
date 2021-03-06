# time control starts
import time as timer
print(timer.ctime())


#import kratos core and applications
#TODO remove import *!

import KratosMultiphysics as km
from KratosMultiphysics.StructuralMechanicsApplication import *
from KratosMultiphysics.TopologyOptimizationApplication import *
from KratosMultiphysics.LinearSolversApplication import *

# For GID output
from KratosMultiphysics.gid_output_process import GiDOutputProcess

# GiD output settings
nodal_results=[]
gauss_points_results=["X_PHYS"]
VolumeOutput = True
GiDPostMode = "Binary"
GiDWriteMeshFlag = False
GiDWriteConditionsFlag = True
GiDWriteParticlesFlag = False
GiDMultiFileFlag = "Single"
       
# Read optimized model part from restart file
current_model = km.Model()
optimized_model_part = current_model.CreateModelPart("optimized_model_part")
optimized_model_part.AddNodalSolutionStepVariable(km.NORMAL)
restart_file_name = "Small_Cantilever_Restart_File_26"
model_part_io = km.ModelPartIO(restart_file_name)
model_part_io.ReadModelPart(optimized_model_part)

# Extract volume
threshold = 0.2
extracted_volume_model_part = current_model.CreateModelPart("extracted_volume_model_part")
TopologyExtractorUtilities().ExtractVolumeMesh(optimized_model_part, threshold, extracted_volume_model_part)

# Write extracted volume
gid_io = GiDOutputProcess(extracted_volume_model_part, "extracted_volume_model_part", 
                                    km.Parameters("""
                                        {
                                            "result_file_configuration" : {
                                                "gidpost_flags": {
                                                    "GiDPostMode": "GiD_PostBinary",
                                                    "WriteDeformedMeshFlag": "WriteUndeformed",
                                                    "WriteConditionsFlag": "WriteConditions",
                                                    "MultiFileFlag": "SingleFile"
                                                },
                                                "nodal_results"       : ["X_PHYS"]
                                            }
                                        }
                                        """)
                                    )
#gid_io.initialize_results(extracted_volume_model_part)
#gid_io.write_results(1, extracted_volume_model_part, nodal_results, gauss_points_results)
#gid_io.finalize_results()
gid_io.ExecuteInitialize()
#gid_io.ExecuteBeforeSolutionLoop()
#gid_io.ExecuteInitializeSolutionStep()
gid_io.PrintOutput()
#gid_io.ExecuteFinalizeSolutionStep()
gid_io.ExecuteFinalize()

# Extract surface
extracted_surface_model_part = current_model.CreateModelPart("extracted_surface_model_part")
TopologyExtractorUtilities().ExtractSurfaceMesh(extracted_volume_model_part, extracted_surface_model_part)

# Write extracted surface
#gid_io_2 = GiDOutputProcess(extracted_volume_model_part,"extracted_surface_model_part",
 #                                   km.Parameters("""
  #                                      {
   #                                         "result_file_configuration" : {
    #                                            "gidpost_flags": {
     #                                               "GiDPostMode": "GiD_PostBinary",
      #                                              "WriteDeformedMeshFlag": "WriteUndeformed",
       #                                             "WriteConditionsFlag": "WriteConditions",
        #                                            "MultiFileFlag": "SingleFile"
         #                                       },
          #                                      "nodal_results"       : ["X_PHYS"]
           #                                 }
            #                            }
             #                           """)
              #s                      )
#gid_io_2.initialize_results(extracted_surface_model_part)
#gid_io_2.write_results(1, extracted_surface_model_part, nodal_results, gauss_points_results)
#gid_io_2.finalize_results()

#gid_io.ExecuteInitialize()
#gid_io.ExecuteBeforeSolutionLoop()
#gid_io.ExecuteInitializeSolutionStep()
#gid_io.PrintOutput()
#gid_io.ExecuteFinalizeSolutionStep()
#gid_io.ExecuteFinalize()


# Smooth extracted surface
smoothing_relaxation_factor = 1
smoothing_iterations = 5
#smoothed_surface_model_part = current_model.ModelPart("extracted_surface_model_part")
#TopologySmoothingUtilities().SmoothMesh(extracted_surface_model_part, smoothing_relaxation_factor, smoothing_iterations)

# Write smoothed mesh
#gid_io_3 = GiDOutputProcess("smoothed_surface_model_part", VolumeOutput, GiDPostMode, GiDMultiFileFlag, GiDWriteMeshFlag, GiDWriteConditionsFlag)
#gid_io_3.initialize_results(extracted_surface_model_part)
#gid_io_3.write_results(1, extracted_surface_model_part, nodal_results, gauss_points_results)
#gid_io_3.finalize_results()