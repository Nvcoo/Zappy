cmake_minimum_required(VERSION 3.16)
project(zappy_gui)

set(CMAKE_CXX_STANDARD 17)

# raylib
find_package(raylib REQUIRED)

# source files
file(GLOB_RECURSE SOURCES
    src/*.cpp
)

add_executable(zappy_gui ${SOURCES})

target_include_directories(zappy_gui PRIVATE src)
target_link_libraries(zappy_gui PRIVATE raylib)
