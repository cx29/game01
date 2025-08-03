cmake_minimum_required(VERSION 3.30)
project(ManagerLib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Interface)

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/Interface/*.cpp" ,
        "${CMAKE_CURRENT_SOURCE_DIR}/UIComponent/*.cpp")

add_library(ManagerLib ${SOURCES}
        UIComponent/MenuPanel.cpp
        UIComponent/ContentPanel.cpp
        Interface/UIManager.cpp)

target_include_directories(ManagerLib
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Interface
        PRIVATE ${SDL2_INCLUDE_DIRS} ${SDL2_ttf_DIR} ${SDL2_mixer_DIR} ${SDL2_image_DIR}
)

target_link_libraries(ManagerLib
        PRIVATE SDL2::SDL2 SDL2_ttf::SDL2_ttf-static SDL2_image::SDL2_image-static SDL2_mixer::SDL2_mixer-static SDL2::SDL2_gfx
)
