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
include public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/depend.make

# Include the progress variables for this target.
include public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/progress.make

# Include the compile flags for this target's objects.
include public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/flags.make

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.o: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/flags.make
public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.o: /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dapp.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.o"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.o -c /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dapp.cpp

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.i"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dapp.cpp > CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.i

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.s"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dapp.cpp -o CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.s

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.o: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/flags.make
public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.o: /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dmain.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.o"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.o -c /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dmain.cpp

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.i"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dmain.cpp > CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.i

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.s"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/fsgui3dmain.cpp -o CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.s

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/singleton.cpp.o: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/flags.make
public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/singleton.cpp.o: /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/singleton.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/singleton.cpp.o"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/fsgui3d_template.dir/singleton.cpp.o -c /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/singleton.cpp

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/singleton.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/fsgui3d_template.dir/singleton.cpp.i"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/singleton.cpp > CMakeFiles/fsgui3d_template.dir/singleton.cpp.i

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/singleton.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/fsgui3d_template.dir/singleton.cpp.s"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template/singleton.cpp -o CMakeFiles/fsgui3d_template.dir/singleton.cpp.s

# Object files for target fsgui3d_template
fsgui3d_template_OBJECTS = \
"CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.o" \
"CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.o" \
"CMakeFiles/fsgui3d_template.dir/singleton.cpp.o"

# External object files for target fsgui3d_template
fsgui3d_template_EXTERNAL_OBJECTS =

public/fsgui3d/template/fsgui3d_template: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dapp.cpp.o
public/fsgui3d/template/fsgui3d_template: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/fsgui3dmain.cpp.o
public/fsgui3d/template/fsgui3d_template: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/singleton.cpp.o
public/fsgui3d/template/fsgui3d_template: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/build.make
public/fsgui3d/template/fsgui3d_template: public/fslazywindow/src/libfslazywindow.a
public/fsgui3d/template/fsgui3d_template: public/ysgl/src/libysgl.a
public/fsgui3d/template/fsgui3d_template: public/fsgui3d/src/libfsgui3d.a
public/fsgui3d/template/fsgui3d_template: public/fsguilib/src/gl2/libfsguilib_gl2.a
public/fsgui3d/template/fsgui3d_template: public/fsgui3d/src/gl2/libfsgui3d_gl2.a
public/fsgui3d/template/fsgui3d_template: public/fsguilib/src/libfsguilib.a
public/fsgui3d/template/fsgui3d_template: public/ysfontrenderer/src/libysfontrenderer.a
public/fsgui3d/template/fsgui3d_template: public/fsguilib/src/gl2/libfsguilib_gl2.a
public/fsgui3d/template/fsgui3d_template: public/yssystemfont/src/libyssystemfont.a
public/fsgui3d/template/fsgui3d_template: public/ysclass/src/libysclass.a
public/fsgui3d/template/fsgui3d_template: public/ysbitmap/src/libysbitmap.a
public/fsgui3d/template/fsgui3d_template: public/fssimplewindow/src/libfssimplewindow.a
public/fsgui3d/template/fsgui3d_template: public/ysbitmapfont/src/libysbitmapfont.a
public/fsgui3d/template/fsgui3d_template: public/ysgl/src/libysgl.a
public/fsgui3d/template/fsgui3d_template: public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable fsgui3d_template"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/fsgui3d_template.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/build: public/fsgui3d/template/fsgui3d_template

.PHONY : public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/build

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/clean:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template && $(CMAKE_COMMAND) -P CMakeFiles/fsgui3d_template.dir/cmake_clean.cmake
.PHONY : public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/clean

public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/depend:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/uas-laptop/adv_eng_comp/sparshg/ps2 /home/uas-laptop/adv_eng_comp/public/src/fsgui3d/template /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : public/fsgui3d/template/CMakeFiles/fsgui3d_template.dir/depend

