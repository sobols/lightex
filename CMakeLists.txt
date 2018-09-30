cmake_minimum_required(VERSION 3.2)

# Define C++ variables
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include_directories(.)

add_subdirectory(contrib)
add_subdirectory(library)
add_subdirectory(lightex)
add_subdirectory(tools)
add_subdirectory(util)
