# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_SOURCE_DIR = /home/joel/src/cpp/JChessV2

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/joel/src/cpp/JChessV2/build

# Include any dependencies generated for this target.
include CMakeFiles/chessboard.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/chessboard.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/chessboard.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/chessboard.dir/flags.make

CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o: CMakeFiles/chessboard.dir/flags.make
CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o: /home/joel/src/cpp/JChessV2/src/common/chessboard/BoardRep.cpp
CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o: CMakeFiles/chessboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joel/src/cpp/JChessV2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o -MF CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o.d -o CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o -c /home/joel/src/cpp/JChessV2/src/common/chessboard/BoardRep.cpp

CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joel/src/cpp/JChessV2/src/common/chessboard/BoardRep.cpp > CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.i

CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joel/src/cpp/JChessV2/src/common/chessboard/BoardRep.cpp -o CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.s

CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o: CMakeFiles/chessboard.dir/flags.make
CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o: /home/joel/src/cpp/JChessV2/src/common/chessboard/dir_table.cpp
CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o: CMakeFiles/chessboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joel/src/cpp/JChessV2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o -MF CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o.d -o CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o -c /home/joel/src/cpp/JChessV2/src/common/chessboard/dir_table.cpp

CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joel/src/cpp/JChessV2/src/common/chessboard/dir_table.cpp > CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.i

CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joel/src/cpp/JChessV2/src/common/chessboard/dir_table.cpp -o CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.s

CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o: CMakeFiles/chessboard.dir/flags.make
CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o: /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/magics.cpp
CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o: CMakeFiles/chessboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joel/src/cpp/JChessV2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o -MF CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o.d -o CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o -c /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/magics.cpp

CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/magics.cpp > CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.i

CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/magics.cpp -o CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.s

CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o: CMakeFiles/chessboard.dir/flags.make
CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o: /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/move_tables.cpp
CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o: CMakeFiles/chessboard.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/joel/src/cpp/JChessV2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o -MF CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o.d -o CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o -c /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/move_tables.cpp

CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.i"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/move_tables.cpp > CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.i

CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.s"
	/usr/lib64/ccache/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/joel/src/cpp/JChessV2/src/common/chessboard/move_gen/move_tables.cpp -o CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.s

# Object files for target chessboard
chessboard_OBJECTS = \
"CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o" \
"CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o" \
"CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o" \
"CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o"

# External object files for target chessboard
chessboard_EXTERNAL_OBJECTS =

lib/libchessboard.a: CMakeFiles/chessboard.dir/src/common/chessboard/BoardRep.cpp.o
lib/libchessboard.a: CMakeFiles/chessboard.dir/src/common/chessboard/dir_table.cpp.o
lib/libchessboard.a: CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/magics.cpp.o
lib/libchessboard.a: CMakeFiles/chessboard.dir/src/common/chessboard/move_gen/move_tables.cpp.o
lib/libchessboard.a: CMakeFiles/chessboard.dir/build.make
lib/libchessboard.a: CMakeFiles/chessboard.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/joel/src/cpp/JChessV2/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library lib/libchessboard.a"
	$(CMAKE_COMMAND) -P CMakeFiles/chessboard.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/chessboard.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/chessboard.dir/build: lib/libchessboard.a
.PHONY : CMakeFiles/chessboard.dir/build

CMakeFiles/chessboard.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/chessboard.dir/cmake_clean.cmake
.PHONY : CMakeFiles/chessboard.dir/clean

CMakeFiles/chessboard.dir/depend:
	cd /home/joel/src/cpp/JChessV2/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/joel/src/cpp/JChessV2 /home/joel/src/cpp/JChessV2 /home/joel/src/cpp/JChessV2/build /home/joel/src/cpp/JChessV2/build /home/joel/src/cpp/JChessV2/build/CMakeFiles/chessboard.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/chessboard.dir/depend
