# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.18

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/lk/30dayServer/day13

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/lk/30dayServer/day13/build

# Include any dependencies generated for this target.
include test/CMakeFiles/single_client.dir/depend.make

# Include the progress variables for this target.
include test/CMakeFiles/single_client.dir/progress.make

# Include the compile flags for this target's objects.
include test/CMakeFiles/single_client.dir/flags.make

test/CMakeFiles/single_client.dir/single_client.cpp.o: test/CMakeFiles/single_client.dir/flags.make
test/CMakeFiles/single_client.dir/single_client.cpp.o: ../test/single_client.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/lk/30dayServer/day13/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object test/CMakeFiles/single_client.dir/single_client.cpp.o"
	cd /home/lk/30dayServer/day13/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/single_client.dir/single_client.cpp.o -c /home/lk/30dayServer/day13/test/single_client.cpp

test/CMakeFiles/single_client.dir/single_client.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/single_client.dir/single_client.cpp.i"
	cd /home/lk/30dayServer/day13/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/lk/30dayServer/day13/test/single_client.cpp > CMakeFiles/single_client.dir/single_client.cpp.i

test/CMakeFiles/single_client.dir/single_client.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/single_client.dir/single_client.cpp.s"
	cd /home/lk/30dayServer/day13/build/test && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/lk/30dayServer/day13/test/single_client.cpp -o CMakeFiles/single_client.dir/single_client.cpp.s

# Object files for target single_client
single_client_OBJECTS = \
"CMakeFiles/single_client.dir/single_client.cpp.o"

# External object files for target single_client
single_client_EXTERNAL_OBJECTS =

bin/single_client: test/CMakeFiles/single_client.dir/single_client.cpp.o
bin/single_client: test/CMakeFiles/single_client.dir/build.make
bin/single_client: lib/libpine_shared.so
bin/single_client: test/CMakeFiles/single_client.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/lk/30dayServer/day13/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ../bin/single_client"
	cd /home/lk/30dayServer/day13/build/test && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/single_client.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
test/CMakeFiles/single_client.dir/build: bin/single_client

.PHONY : test/CMakeFiles/single_client.dir/build

test/CMakeFiles/single_client.dir/clean:
	cd /home/lk/30dayServer/day13/build/test && $(CMAKE_COMMAND) -P CMakeFiles/single_client.dir/cmake_clean.cmake
.PHONY : test/CMakeFiles/single_client.dir/clean

test/CMakeFiles/single_client.dir/depend:
	cd /home/lk/30dayServer/day13/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/lk/30dayServer/day13 /home/lk/30dayServer/day13/test /home/lk/30dayServer/day13/build /home/lk/30dayServer/day13/build/test /home/lk/30dayServer/day13/build/test/CMakeFiles/single_client.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : test/CMakeFiles/single_client.dir/depend

