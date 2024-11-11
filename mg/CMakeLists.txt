cmake_minimum_required(VERSION 3.25)

project(mg VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(sdlwrapper REQUIRED)

add_library(mg include/MG.hpp src/MG.cpp)
target_link_libraries(mg PRIVATE sdlwrapper::sdlwrapper)


