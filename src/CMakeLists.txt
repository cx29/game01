cmake_minimum_required(VERSION 3.30)
project(ManagerLib)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/Interface)

file(GLOB_RECURSE SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/Interface/*.cpp")

add_library(ManagerLib ${SOURCES})

target_include_directories(ManagerLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/Interface)
