cmake_minimum_required(VERSION 3.29)
project(game01 CXX)

set(CMAKE_CXX_STANDARD  11)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE "$ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake" CACHE STRING "")

find_package(SDL2 CONFIG REQUIRED)
find_package(SDL2_ttf CONFIG REQUIRED)
find_package(SDL2_mixer CONFIG REQUIRED)
find_package(SDL2_image CONFIG REQUIRED)
find_package(SDL2-gfx CONFIG REQUIRED)

message(STATUS "SDL2-gfx found at: ${SDL2-gfx_DIR}")
add_subdirectory(src)

add_executable(game01 MACOSX_BUNDLE src/main.cpp)

add_custom_command(TARGET game01 POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/resource $<TARGET_FILE_DIR:game01>/resource
)

# 链接库
target_link_libraries(game01
        SDL2::SDL2
        SDL2_ttf::SDL2_ttf-static
        SDL2_mixer::SDL2_mixer-static
        SDL2_image::SDL2_image-static
        SDL2::SDL2_gfx
        ManagerLib
)


