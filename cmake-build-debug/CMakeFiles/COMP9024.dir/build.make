# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/guohaojin/Documents/COMP9315_21T1

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/guohaojin/Documents/COMP9315_21T1/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/COMP9024.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/COMP9024.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/COMP9024.dir/flags.make

CMakeFiles/COMP9024.dir/Codes/practice.c.o: CMakeFiles/COMP9024.dir/flags.make
CMakeFiles/COMP9024.dir/Codes/practice.c.o: ../Codes/practice.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/guohaojin/Documents/COMP9315_21T1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/COMP9024.dir/Codes/practice.c.o"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/COMP9024.dir/Codes/practice.c.o   -c /Users/guohaojin/Documents/COMP9315_21T1/Codes/practice.c

CMakeFiles/COMP9024.dir/Codes/practice.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/COMP9024.dir/Codes/practice.c.i"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/guohaojin/Documents/COMP9315_21T1/Codes/practice.c > CMakeFiles/COMP9024.dir/Codes/practice.c.i

CMakeFiles/COMP9024.dir/Codes/practice.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/COMP9024.dir/Codes/practice.c.s"
	/Library/Developer/CommandLineTools/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/guohaojin/Documents/COMP9315_21T1/Codes/practice.c -o CMakeFiles/COMP9024.dir/Codes/practice.c.s

# Object files for target COMP9024
COMP9024_OBJECTS = \
"CMakeFiles/COMP9024.dir/Codes/practice.c.o"

# External object files for target COMP9024
COMP9024_EXTERNAL_OBJECTS =

COMP9024: CMakeFiles/COMP9024.dir/Codes/practice.c.o
COMP9024: CMakeFiles/COMP9024.dir/build.make
COMP9024: CMakeFiles/COMP9024.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/guohaojin/Documents/COMP9315_21T1/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable COMP9024"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/COMP9024.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/COMP9024.dir/build: COMP9024

.PHONY : CMakeFiles/COMP9024.dir/build

CMakeFiles/COMP9024.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/COMP9024.dir/cmake_clean.cmake
.PHONY : CMakeFiles/COMP9024.dir/clean

CMakeFiles/COMP9024.dir/depend:
	cd /Users/guohaojin/Documents/COMP9315_21T1/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/guohaojin/Documents/COMP9315_21T1 /Users/guohaojin/Documents/COMP9315_21T1 /Users/guohaojin/Documents/COMP9315_21T1/cmake-build-debug /Users/guohaojin/Documents/COMP9315_21T1/cmake-build-debug /Users/guohaojin/Documents/COMP9315_21T1/cmake-build-debug/CMakeFiles/COMP9024.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/COMP9024.dir/depend

