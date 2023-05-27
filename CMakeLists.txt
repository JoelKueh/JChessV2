# Set min required version
cmake_minimum_required(VERSION 3.10)

# Set project name
project(JChess VERSION 2.0)

# Ncurses stuff
set(CURSES_NEED_NCURSES TRUE)
set(CURSES_NEED_WIDE TRUE)
include(FindCurses)
include_directories(${NCURSES_INCLUDE_DIRS})

# GLFW stuff
find_package(glfw3 REQUIRED)

# Build chessboard library
add_library(chessboard src/common/chessboard/BoardRep.cpp
	src/common/chessboard/dir_table.cpp
	src/common/chessboard/move_gen/magics.cpp
	src/common/chessboard/move_gen/move_tables.cpp)

# Force CMake to output compile commands
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

# Build old debug executable
add_executable(old_debug src/debug/old/JChessV2.cpp
	src/debug/old/scenes/Scene.cpp
	src/debug/old/scenes/Game/Game.cpp
	src/debug/old/scenes/Game/Move.cpp
	src/debug/old/scenes/Game/UI/GameCLI.cpp
	src/debug/old/scenes/Game/UI/GameUI.cpp
	src/debug/old/scenes/Game/UI/GameGUI.cpp
	src/debug/old/scenes/Game/Player/Player.cpp
	src/debug/old/scenes/Game/Player/PlayerAI.cpp
	src/debug/old/scenes/Game/Player/PlayerLocal.cpp
	src/debug/old/scenes/Game/Player/PlayerRemote.cpp
	src/debug/old/scenes/Game/Player/PlayerTheFish.cpp
	src/debug/old/scenes/StartMenu/StartMenu.cpp
	src/debug/old/scenes/StartMenu/StartUI.cpp
	src/debug/old/scenes/StartMenu/StartCLI.cpp
	src/debug/old/scenes/StartMenu/StartGUI.cpp)
target_link_libraries(old_debug chessboard ${NCURSES_LIBRARIES} glfw)

# Set output path
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
