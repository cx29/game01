cmake_minimum_required(VERSION 3.30)
project(game01)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)


# 包含头文件
include_directories(
        ${CMAKE_SOURCE_DIR}/include/
        ${CMAKE_SOURCE_DIR}/include/cjson
        ${CMAKE_SOURCE_DIR}/include/SDL2
)
link_directories(${CMAKE_SOURCE_DIR}/lib)

add_subdirectory(src)
add_executable(game01 ./src/main.cpp)

add_custom_command(TARGET game01 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/resource $<TARGET_FILE_DIR:game01>/resource
)

# 链接库
target_link_libraries(game01
        SDL2
        SDL2_ttf
        SDL2_mixer
        SDL2_image
        SDL2_gfx
        cJSON
        cJSON_utils
        ManagerLib
)

