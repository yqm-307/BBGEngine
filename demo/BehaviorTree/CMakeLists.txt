cmake_minimum_required(VERSION 3.1)

project(foodtree)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 -g ${Wall_Flag}")

file(GLOB_RECURSE CPP_SRC 
    ${PROJECT_SOURCE_DIR}/*.cc
)

add_executable(foodtree
    ${CPP_SRC}
)