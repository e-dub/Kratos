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

# Utility rule file for mmg3d_fortran_header.

# Include the progress variables for this target.
include CMakeFiles/mmg3d_fortran_header.dir/progress.make

CMakeFiles/mmg3d_fortran_header: src/mmg3d/libmmg3df.h


src/mmg3d/libmmg3df.h: bin/genheader
src/mmg3d/libmmg3df.h: ../src/mmg3d/libmmg3d.h
src/mmg3d/libmmg3df.h: ../scripts/genfort.pl
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating Fortran header for mmg3d"
	/home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build/bin/genheader /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build/src/mmg3d/libmmg3df.h /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/src/mmg3d/libmmg3d.h mmg/mmg3d /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/scripts/genfort.pl

mmg3d_fortran_header: CMakeFiles/mmg3d_fortran_header
mmg3d_fortran_header: src/mmg3d/libmmg3df.h
mmg3d_fortran_header: CMakeFiles/mmg3d_fortran_header.dir/build.make

.PHONY : mmg3d_fortran_header

# Rule to build all files generated by this target.
CMakeFiles/mmg3d_fortran_header.dir/build: mmg3d_fortran_header

.PHONY : CMakeFiles/mmg3d_fortran_header.dir/build

CMakeFiles/mmg3d_fortran_header.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/mmg3d_fortran_header.dir/cmake_clean.cmake
.PHONY : CMakeFiles/mmg3d_fortran_header.dir/clean

CMakeFiles/mmg3d_fortran_header.dir/depend:
	cd /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build /home/mhashemi/programming/Git/Kratos-working/applications/MeshingApplication/custom_external_libraries/mmg/build/CMakeFiles/mmg3d_fortran_header.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/mmg3d_fortran_header.dir/depend

