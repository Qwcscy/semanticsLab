# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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
CMAKE_COMMAND = /usr/share/clion/bin/cmake/bin/cmake

# The command to remove a file.
RM = /usr/share/clion/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/scy/Documents/compilingLab2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/scy/Documents/compilingLab2/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/compilingLab2.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/compilingLab2.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/compilingLab2.dir/flags.make

CMakeFiles/compilingLab2.dir/main.cpp.o: CMakeFiles/compilingLab2.dir/flags.make
CMakeFiles/compilingLab2.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/scy/Documents/compilingLab2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/compilingLab2.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compilingLab2.dir/main.cpp.o -c /home/scy/Documents/compilingLab2/main.cpp

CMakeFiles/compilingLab2.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compilingLab2.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/scy/Documents/compilingLab2/main.cpp > CMakeFiles/compilingLab2.dir/main.cpp.i

CMakeFiles/compilingLab2.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compilingLab2.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/scy/Documents/compilingLab2/main.cpp -o CMakeFiles/compilingLab2.dir/main.cpp.s

CMakeFiles/compilingLab2.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/compilingLab2.dir/main.cpp.o.requires

CMakeFiles/compilingLab2.dir/main.cpp.o.provides: CMakeFiles/compilingLab2.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/compilingLab2.dir/build.make CMakeFiles/compilingLab2.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/compilingLab2.dir/main.cpp.o.provides

CMakeFiles/compilingLab2.dir/main.cpp.o.provides.build: CMakeFiles/compilingLab2.dir/main.cpp.o


CMakeFiles/compilingLab2.dir/symbolTable.cpp.o: CMakeFiles/compilingLab2.dir/flags.make
CMakeFiles/compilingLab2.dir/symbolTable.cpp.o: ../symbolTable.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/scy/Documents/compilingLab2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/compilingLab2.dir/symbolTable.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/compilingLab2.dir/symbolTable.cpp.o -c /home/scy/Documents/compilingLab2/symbolTable.cpp

CMakeFiles/compilingLab2.dir/symbolTable.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/compilingLab2.dir/symbolTable.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/scy/Documents/compilingLab2/symbolTable.cpp > CMakeFiles/compilingLab2.dir/symbolTable.cpp.i

CMakeFiles/compilingLab2.dir/symbolTable.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/compilingLab2.dir/symbolTable.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/scy/Documents/compilingLab2/symbolTable.cpp -o CMakeFiles/compilingLab2.dir/symbolTable.cpp.s

CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.requires:

.PHONY : CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.requires

CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.provides: CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.requires
	$(MAKE) -f CMakeFiles/compilingLab2.dir/build.make CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.provides.build
.PHONY : CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.provides

CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.provides.build: CMakeFiles/compilingLab2.dir/symbolTable.cpp.o


# Object files for target compilingLab2
compilingLab2_OBJECTS = \
"CMakeFiles/compilingLab2.dir/main.cpp.o" \
"CMakeFiles/compilingLab2.dir/symbolTable.cpp.o"

# External object files for target compilingLab2
compilingLab2_EXTERNAL_OBJECTS =

compilingLab2: CMakeFiles/compilingLab2.dir/main.cpp.o
compilingLab2: CMakeFiles/compilingLab2.dir/symbolTable.cpp.o
compilingLab2: CMakeFiles/compilingLab2.dir/build.make
compilingLab2: CMakeFiles/compilingLab2.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/scy/Documents/compilingLab2/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable compilingLab2"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/compilingLab2.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/compilingLab2.dir/build: compilingLab2

.PHONY : CMakeFiles/compilingLab2.dir/build

CMakeFiles/compilingLab2.dir/requires: CMakeFiles/compilingLab2.dir/main.cpp.o.requires
CMakeFiles/compilingLab2.dir/requires: CMakeFiles/compilingLab2.dir/symbolTable.cpp.o.requires

.PHONY : CMakeFiles/compilingLab2.dir/requires

CMakeFiles/compilingLab2.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/compilingLab2.dir/cmake_clean.cmake
.PHONY : CMakeFiles/compilingLab2.dir/clean

CMakeFiles/compilingLab2.dir/depend:
	cd /home/scy/Documents/compilingLab2/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/scy/Documents/compilingLab2 /home/scy/Documents/compilingLab2 /home/scy/Documents/compilingLab2/cmake-build-debug /home/scy/Documents/compilingLab2/cmake-build-debug /home/scy/Documents/compilingLab2/cmake-build-debug/CMakeFiles/compilingLab2.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/compilingLab2.dir/depend

