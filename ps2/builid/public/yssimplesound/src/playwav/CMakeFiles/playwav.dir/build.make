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
include public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/depend.make

# Include the progress variables for this target.
include public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/progress.make

# Include the compile flags for this target's objects.
include public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/flags.make

public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/playwav.cpp.o: public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/flags.make
public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/playwav.cpp.o: /home/uas-laptop/adv_eng_comp/public/src/yssimplesound/src/playwav/playwav.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/playwav.cpp.o"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/yssimplesound/src/playwav && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/playwav.dir/playwav.cpp.o -c /home/uas-laptop/adv_eng_comp/public/src/yssimplesound/src/playwav/playwav.cpp

public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/playwav.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/playwav.dir/playwav.cpp.i"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/yssimplesound/src/playwav && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/uas-laptop/adv_eng_comp/public/src/yssimplesound/src/playwav/playwav.cpp > CMakeFiles/playwav.dir/playwav.cpp.i

public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/playwav.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/playwav.dir/playwav.cpp.s"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/yssimplesound/src/playwav && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/uas-laptop/adv_eng_comp/public/src/yssimplesound/src/playwav/playwav.cpp -o CMakeFiles/playwav.dir/playwav.cpp.s

# Object files for target playwav
playwav_OBJECTS = \
"CMakeFiles/playwav.dir/playwav.cpp.o"

# External object files for target playwav
playwav_EXTERNAL_OBJECTS =

public/yssimplesound/src/playwav/playwav: public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/playwav.cpp.o
public/yssimplesound/src/playwav/playwav: public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/build.make
public/yssimplesound/src/playwav/playwav: public/fssimplewindow/src/libfssimplewindow.a
public/yssimplesound/src/playwav/playwav: public/yssimplesound/src/libyssimplesound.a
public/yssimplesound/src/playwav/playwav: public/yssimplesound/src/libyssimplesound_linux_alsa.a
public/yssimplesound/src/playwav/playwav: /usr/lib/x86_64-linux-gnu/libasound.so
public/yssimplesound/src/playwav/playwav: public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable playwav"
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/yssimplesound/src/playwav && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/playwav.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/build: public/yssimplesound/src/playwav/playwav

.PHONY : public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/build

public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/clean:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/yssimplesound/src/playwav && $(CMAKE_COMMAND) -P CMakeFiles/playwav.dir/cmake_clean.cmake
.PHONY : public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/clean

public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/depend:
	cd /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/uas-laptop/adv_eng_comp/sparshg/ps2 /home/uas-laptop/adv_eng_comp/public/src/yssimplesound/src/playwav /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/yssimplesound/src/playwav /home/uas-laptop/adv_eng_comp/sparshg/ps2/builid/public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : public/yssimplesound/src/playwav/CMakeFiles/playwav.dir/depend

