# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.10

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

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = "C:\Program Files\JetBrains\CLion 2018.1.1\bin\cmake\bin\cmake.exe"

# The command to remove a file.
RM = "C:\Program Files\JetBrains\CLion 2018.1.1\bin\cmake\bin\cmake.exe" -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = C:\Users\MUSE\Desktop\complier

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = C:\Users\MUSE\Desktop\complier\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/complier.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/complier.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/complier.dir/flags.make

CMakeFiles/complier.dir/main.cpp.obj: CMakeFiles/complier.dir/flags.make
CMakeFiles/complier.dir/main.cpp.obj: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\MUSE\Desktop\complier\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/complier.dir/main.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\complier.dir\main.cpp.obj -c C:\Users\MUSE\Desktop\complier\main.cpp

CMakeFiles/complier.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/complier.dir/main.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\MUSE\Desktop\complier\main.cpp > CMakeFiles\complier.dir\main.cpp.i

CMakeFiles/complier.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/complier.dir/main.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\MUSE\Desktop\complier\main.cpp -o CMakeFiles\complier.dir\main.cpp.s

CMakeFiles/complier.dir/main.cpp.obj.requires:

.PHONY : CMakeFiles/complier.dir/main.cpp.obj.requires

CMakeFiles/complier.dir/main.cpp.obj.provides: CMakeFiles/complier.dir/main.cpp.obj.requires
	$(MAKE) -f CMakeFiles\complier.dir\build.make CMakeFiles/complier.dir/main.cpp.obj.provides.build
.PHONY : CMakeFiles/complier.dir/main.cpp.obj.provides

CMakeFiles/complier.dir/main.cpp.obj.provides.build: CMakeFiles/complier.dir/main.cpp.obj


CMakeFiles/complier.dir/lexer.cpp.obj: CMakeFiles/complier.dir/flags.make
CMakeFiles/complier.dir/lexer.cpp.obj: ../lexer.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=C:\Users\MUSE\Desktop\complier\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/complier.dir/lexer.cpp.obj"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles\complier.dir\lexer.cpp.obj -c C:\Users\MUSE\Desktop\complier\lexer.cpp

CMakeFiles/complier.dir/lexer.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/complier.dir/lexer.cpp.i"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E C:\Users\MUSE\Desktop\complier\lexer.cpp > CMakeFiles\complier.dir\lexer.cpp.i

CMakeFiles/complier.dir/lexer.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/complier.dir/lexer.cpp.s"
	C:\PROGRA~1\MINGW-~1\X86_64~1.0-P\mingw64\bin\G__~1.EXE $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S C:\Users\MUSE\Desktop\complier\lexer.cpp -o CMakeFiles\complier.dir\lexer.cpp.s

CMakeFiles/complier.dir/lexer.cpp.obj.requires:

.PHONY : CMakeFiles/complier.dir/lexer.cpp.obj.requires

CMakeFiles/complier.dir/lexer.cpp.obj.provides: CMakeFiles/complier.dir/lexer.cpp.obj.requires
	$(MAKE) -f CMakeFiles\complier.dir\build.make CMakeFiles/complier.dir/lexer.cpp.obj.provides.build
.PHONY : CMakeFiles/complier.dir/lexer.cpp.obj.provides

CMakeFiles/complier.dir/lexer.cpp.obj.provides.build: CMakeFiles/complier.dir/lexer.cpp.obj


# Object files for target complier
complier_OBJECTS = \
"CMakeFiles/complier.dir/main.cpp.obj" \
"CMakeFiles/complier.dir/lexer.cpp.obj"

# External object files for target complier
complier_EXTERNAL_OBJECTS =

complier.exe: CMakeFiles/complier.dir/main.cpp.obj
complier.exe: CMakeFiles/complier.dir/lexer.cpp.obj
complier.exe: CMakeFiles/complier.dir/build.make
complier.exe: CMakeFiles/complier.dir/linklibs.rsp
complier.exe: CMakeFiles/complier.dir/objects1.rsp
complier.exe: CMakeFiles/complier.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=C:\Users\MUSE\Desktop\complier\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable complier.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\complier.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/complier.dir/build: complier.exe

.PHONY : CMakeFiles/complier.dir/build

CMakeFiles/complier.dir/requires: CMakeFiles/complier.dir/main.cpp.obj.requires
CMakeFiles/complier.dir/requires: CMakeFiles/complier.dir/lexer.cpp.obj.requires

.PHONY : CMakeFiles/complier.dir/requires

CMakeFiles/complier.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\complier.dir\cmake_clean.cmake
.PHONY : CMakeFiles/complier.dir/clean

CMakeFiles/complier.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" C:\Users\MUSE\Desktop\complier C:\Users\MUSE\Desktop\complier C:\Users\MUSE\Desktop\complier\cmake-build-debug C:\Users\MUSE\Desktop\complier\cmake-build-debug C:\Users\MUSE\Desktop\complier\cmake-build-debug\CMakeFiles\complier.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/complier.dir/depend

