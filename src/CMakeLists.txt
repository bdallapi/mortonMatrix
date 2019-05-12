cmake_minimum_required (VERSION 2.8) 
cmake_policy(SET CMP0028 NEW)

project (mortonMatrix) 

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Boost COMPONENTS program_options REQUIRED)

add_subdirectory (mortonMatrix) 
add_subdirectory (app) 
add_subdirectory (extern/gtest)

