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
include public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/depend.make

# Include the progress variables for this target.
include public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/progress.make

# Include the compile flags for this target's objects.
include public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/flags.make

public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/main.cpp.o: public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/flags.make
public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/main.cpp.o: /home/uas-laptop/adv_eng_comp/public/src/ysgebl/src/samples/YsShellExt/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/main.cpp.o"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/ysgebl/src/samples/YsShellExt && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/ysshellext_sample.dir/main.cpp.o -c /home/uas-laptop/adv_eng_comp/public/src/ysgebl/src/samples/YsShellExt/main.cpp

public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/ysshellext_sample.dir/main.cpp.i"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/ysgebl/src/samples/YsShellExt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/uas-laptop/adv_eng_comp/public/src/ysgebl/src/samples/YsShellExt/main.cpp > CMakeFiles/ysshellext_sample.dir/main.cpp.i

public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/ysshellext_sample.dir/main.cpp.s"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/ysgebl/src/samples/YsShellExt && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/uas-laptop/adv_eng_comp/public/src/ysgebl/src/samples/YsShellExt/main.cpp -o CMakeFiles/ysshellext_sample.dir/main.cpp.s

# Object files for target ysshellext_sample
ysshellext_sample_OBJECTS = \
"CMakeFiles/ysshellext_sample.dir/main.cpp.o"

# External object files for target ysshellext_sample
ysshellext_sample_EXTERNAL_OBJECTS =

public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/main.cpp.o
public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/build.make
public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/ysgebl/src/kernel/libgeblkernel.a
public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/fssimplewindow/src/libfssimplewindow.a
public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/ysglcpp/src/libysglcpp.a
public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/ysclass11/src/libysclass11.a
public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/ysclass/src/libysclass.a
public/ysgebl/src/samples/YsShellExt/ysshellext_sample: public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable ysshellext_sample"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/ysgebl/src/samples/YsShellExt && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/ysshellext_sample.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/build: public/ysgebl/src/samples/YsShellExt/ysshellext_sample

.PHONY : public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/build

public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/clean:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/ysgebl/src/samples/YsShellExt && $(CMAKE_COMMAND) -P CMakeFiles/ysshellext_sample.dir/cmake_clean.cmake
.PHONY : public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/clean

public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/depend:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/uas-laptop/adv_eng_comp/sparshg/ps2 /home/uas-laptop/adv_eng_comp/public/src/ysgebl/src/samples/YsShellExt /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/ysgebl/src/samples/YsShellExt /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : public/ysgebl/src/samples/YsShellExt/CMakeFiles/ysshellext_sample.dir/depend

