cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
set(CMAKE_BUILD_TYPE Debug)

project(SoftwareRasterizer C)

find_package(SDL2 REQUIRED)

set(SRCS src/main.c src/engine.c src/rasterizer.c src/math.c src/triangle.c)

add_executable(app ${SRCS})

target_include_directories(app PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc)
target_link_libraries(app PRIVATE SDL2::SDL2)
