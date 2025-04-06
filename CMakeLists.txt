cmake_minimum_required(VERSION 3.2)

project(bbt_ecs)

option(NO_EXAMPLE
    "不需要编译例程" ON)

option(NO_TEST
    "不需要编译测试" ON)

option(DEBUG
    "调试信息" ON)

    
if(DEBUG)
    add_definitions(-DDebug)
endif()

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g ${Wall_Flag}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/bbt
)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/bbt/**.cc
)

add_library(bbt_ecs SHARED
    ${SOURCE_FILES}
)

target_link_libraries(bbt_ecs PRIVATE
    bbt_core
)


if(NOT NO_TEST)
    enable_testing()
    add_subdirectory(unit_test)    
endif()