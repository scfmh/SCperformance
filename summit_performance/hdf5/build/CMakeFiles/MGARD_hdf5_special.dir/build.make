# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.23

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /autofs/nccs-svm1_sw/summit/spack-envs/base/opt/linux-rhel8-ppc64le/gcc-8.3.1/cmake-3.23.2-jwqghrfxvfhuozlioasit4bvncwg5yff/bin/cmake

# The command to remove a file.
RM = /autofs/nccs-svm1_sw/summit/spack-envs/base/opt/linux-rhel8-ppc64le/gcc-8.3.1/cmake-3.23.2-jwqghrfxvfhuozlioasit4bvncwg5yff/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/build

# Include any dependencies generated for this target.
include CMakeFiles/MGARD_hdf5_special.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/MGARD_hdf5_special.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/MGARD_hdf5_special.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/MGARD_hdf5_special.dir/flags.make

CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o: CMakeFiles/MGARD_hdf5_special.dir/flags.make
CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o: ../MGARD_hdf5_special.cpp
CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o: CMakeFiles/MGARD_hdf5_special.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o"
	/sw/summit/gcc/9.1.0-alpha+20190716/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o -MF CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o.d -o CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o -c /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/MGARD_hdf5_special.cpp

CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.i"
	/sw/summit/gcc/9.1.0-alpha+20190716/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/MGARD_hdf5_special.cpp > CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.i

CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.s"
	/sw/summit/gcc/9.1.0-alpha+20190716/bin/g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/MGARD_hdf5_special.cpp -o CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.s

# Object files for target MGARD_hdf5_special
MGARD_hdf5_special_OBJECTS = \
"CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o"

# External object files for target MGARD_hdf5_special
MGARD_hdf5_special_EXTERNAL_OBJECTS =

MGARD_hdf5_special: CMakeFiles/MGARD_hdf5_special.dir/MGARD_hdf5_special.cpp.o
MGARD_hdf5_special: CMakeFiles/MGARD_hdf5_special.dir/build.make
MGARD_hdf5_special: /ccs/home/zq53/lossy/mgard/mgard_7_23/MGARD/install-cuda-summit/lib/libmgard.so.1.5.1
MGARD_hdf5_special: /ccs/home/zq53/IO_lib/hdf5_1_14_2/hdf5/install/lib/libhdf5.so
MGARD_hdf5_special: /ccs/home/zq53/lossy/mgard/mgard_7_23/MGARD/install-cuda-summit/lib64/libprotobuf.so
MGARD_hdf5_special: /usr/lib64/libpthread.so
MGARD_hdf5_special: /usr/lib64/libz.so
MGARD_hdf5_special: /ccs/home/zq53/lossy/mgard/mgard_7_23/MGARD/install-cuda-summit/lib/libzstd.so
MGARD_hdf5_special: /ccs/home/zq53/lossy/mgard/mgard_7_23/MGARD/install-cuda-summit/lib/libnvcomp.so
MGARD_hdf5_special: /sw/summit/cuda/11.4.2/lib64/libcudart.so
MGARD_hdf5_special: /sw/summit/cuda/11.4.2/lib64/libnvidia-ml.so
MGARD_hdf5_special: /autofs/nccs-svm1_sw/summit/gcc/9.1.0-alpha+20190716/lib64/libgomp.so
MGARD_hdf5_special: /lib64/libpthread.so
MGARD_hdf5_special: /sw/summit/spack-envs/base/opt/linux-rhel8-ppc64le/gcc-9.1.0/spectrum-mpi-10.4.0.3-20210112-6jbupg3thjwhsabgevk6xmwhd2bbyxdc/lib/libmpiprofilesupport.so
MGARD_hdf5_special: /sw/summit/spack-envs/base/opt/linux-rhel8-ppc64le/gcc-9.1.0/spectrum-mpi-10.4.0.3-20210112-6jbupg3thjwhsabgevk6xmwhd2bbyxdc/lib/libmpi_ibm.so
MGARD_hdf5_special: CMakeFiles/MGARD_hdf5_special.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable MGARD_hdf5_special"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/MGARD_hdf5_special.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/MGARD_hdf5_special.dir/build: MGARD_hdf5_special
.PHONY : CMakeFiles/MGARD_hdf5_special.dir/build

CMakeFiles/MGARD_hdf5_special.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/MGARD_hdf5_special.dir/cmake_clean.cmake
.PHONY : CMakeFiles/MGARD_hdf5_special.dir/clean

CMakeFiles/MGARD_hdf5_special.dir/depend:
	cd /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5 /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5 /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/build /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/build /gpfs/alpine/proj-shared/csc143/zq53/mgard_new/adios_more/IO_test/hdf5/build/CMakeFiles/MGARD_hdf5_special.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/MGARD_hdf5_special.dir/depend
