# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_SOURCE_DIR = /home/uas-laptop/adv_eng_comp/sparshg/ps2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid

# Include any dependencies generated for this target.
include public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/depend.make

# Include the progress variables for this target.
include public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/progress.make

# Include the compile flags for this target's objects.
include public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/flags.make

public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.o: public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/flags.make
public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.o: /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/src/gl1/fsgui3dgl1.x.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.o"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1 && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.o -c /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/src/gl1/fsgui3dgl1.x.cpp

public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.i"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/src/gl1/fsgui3dgl1.x.cpp > CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.i

public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.s"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1 && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/src/gl1/fsgui3dgl1.x.cpp -o CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.s

# Object files for target fsgui3d_gl1
fsgui3d_gl1_OBJECTS = \
"CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.o"

# External object files for target fsgui3d_gl1
fsgui3d_gl1_EXTERNAL_OBJECTS =

public/fsgui3d/src/gl1/libfsgui3d_gl1.a: public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/fsgui3dgl1.x.cpp.o
public/fsgui3d/src/gl1/libfsgui3d_gl1.a: public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/build.make
public/fsgui3d/src/gl1/libfsgui3d_gl1.a: public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libfsgui3d_gl1.a"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1 && $(CMAKE_COMMAND) -P CMakeFiles/fsgui3d_gl1.dir/cmake_clean_target.cmake
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1 && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fsgui3d_gl1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/build: public/fsgui3d/src/gl1/libfsgui3d_gl1.a

.PHONY : public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/build

public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/clean:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1 && $(CMAKE_COMMAND) -P CMakeFiles/fsgui3d_gl1.dir/cmake_clean.cmake
.PHONY : public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/clean

public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/depend:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/uas-laptop/adv_eng_comp/sparshg/ps2 /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/src/gl1 /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1 /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : public/fsgui3d/src/gl1/CMakeFiles/fsgui3d_gl1.dir/depend

