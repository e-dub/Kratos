# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.13

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build

# Utility rule file for copy_2d_headers.

# Include the progress variables for this target.
include CMakeFiles/copy_2d_headers.dir/progress.make

CMakeFiles/copy_2d_headers: include/mmg/mmg2d/libmmg2d.h
CMakeFiles/copy_2d_headers: include/mmg/mmg2d/libmmgtypes.h


copy_2d_headers: CMakeFiles/copy_2d_headers
copy_2d_headers: CMakeFiles/copy_2d_headers.dir/build.make

.PHONY : copy_2d_headers

# Rule to build all files generated by this target.
CMakeFiles/copy_2d_headers.dir/build: copy_2d_headers

.PHONY : CMakeFiles/copy_2d_headers.dir/build

CMakeFiles/copy_2d_headers.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/copy_2d_headers.dir/cmake_clean.cmake
.PHONY : CMakeFiles/copy_2d_headers.dir/clean

CMakeFiles/copy_2d_headers.dir/depend:
	cd /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build/CMakeFiles/copy_2d_headers.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/copy_2d_headers.dir/depend

