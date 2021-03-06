#========================================================================================
# DESIGN VARIABLES
#========================================================================================

optimization_method = "simp_method" 
# options: "simp_method"

# case: optimization_method == "simp_method"
simp_property         	= 1             # Property ID of the material
penalty               	= 3             # Penalty Factor - Recomended: 3
continuation_strategy 	= 1             # Options: Activated=1, Deactivated=0
E_min                 	= 0.000000001   # Elastic modulus of void material
initial_volume_fraction = 0.15			# Initial densitiy distribution
density_filter			= "density"		# density filter active if "density" else unactive

#========================================================================================
# FILTERING OPTIONS FOR SIMP APPROACH
#========================================================================================

filter_type = "sensitivity"
# options: "sensitivity"

filter_kernel = "linear"
# options: "linear"

filter_radius     		  	  = 0.75
max_elements_in_filter_radius = 500 # Defines max number of elements considered within
							        # specified filter radius (reduces memory footprint)

grey_scale_filter = 0 
# options: Activated=1, Deactivated=0

# case: grey_scale_filter = 1
q_max = 2 # Recomended: 2


#========================================================================================
# OPTIMIZATION ALGORITHM
#========================================================================================
    
optimization_algorithm = "oc_algorithm"  #"MMA_algorithm"  #"oc_algorithm" für OC und "MMA_algorithm" für MMA
# options: "oc_algorithm"
    
# General convergence criterions
max_opt_iterations = 100
relative_tolerance = 0.000001      
increasing_obj     = 0 # Stops the optimization when objective function value is increasing
# options: Activated=1, Deactivated=0

  
#========================================================================================
# RESPONSE FUNCTIONS
#========================================================================================

# Define container of objective functions
# Format: objectives = { "unique_func_id": {"grad": "provided"},
#                        "unique_func_id": {"grad": "provided"},
#               		... }
objectives = { "strain_energy": {"grad": "provided"} }

# Define container of constraint functions
# Format: constraints = { "unique_func_id": {"type": "eq"/"ineq", "grad": "provided"},
#                         "unique_func_id": {"type": "eq"/"ineq", "grad": "provided"},
#               		... }    
constraints = { "volume_fraction": {"type": "eq", "grad": "provided"} }

#========================================================================================
# POST-PROCESSING SETTINGS
#========================================================================================

# General output
restart_input_file      = "CablewayPylon.mdpa"
restart_output_file     = "CablewayPylon_Restart_File.mdpa"
restart_write_frequency = 10 #iterations

# GiD simulation output file name
GiD_output_file_name   = "Topology_Optimization_Results"
nodal_results=["DISPLACEMENT","REACTION"]
gauss_points_results=["X_PHYS"]

# GiD output configuration
VolumeOutput = True
GiDPostMode = "Binary"
GiDWriteMeshFlag = False
GiDWriteConditionsFlag = False
GiDWriteParticlesFlag = False
GiDMultiFileFlag = "Single"

#========================================================================================   
