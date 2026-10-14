cmake_minimum_required(VERSION 3.16)
project(minesweeper CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf REQUIRED)

add_executable(minesweeper src/gui.cpp src/board.cpp src/cell.cpp)
target_include_directories(minesweeper PRIVATE src)
target_link_libraries(minesweeper PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf)

add_executable(terminal src/terminal.cpp src/board.cpp src/cell.cpp)
target_include_directories(terminal PRIVATE src)
