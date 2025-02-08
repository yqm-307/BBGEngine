cmake_minimum_required(VERSION 3.2)

project(BBGEngine)

option(NO_EXAMPLE
    "不需要编译例程" ON)

option(NO_TEST
    "不需要编译测试" ON)

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g ${Wall_Flag}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
    ${PROJECT_SOURCE_DIR}/thirdparty/include
)

file(GLOB_RECURSE SOURCE_FILES
    ${PROJECT_SOURCE_DIR}/src/engine/*.cc
    ${PROJECT_SOURCE_DIR}/src/plugin/*.cc
    ${PROJECT_SOURCE_DIR}/src/util/*.cc
)

add_library(BBGEngine SHARED
    ${SOURCE_FILES}
)

target_link_libraries(BBGEngine PRIVATE
    protobuf
    inih
    ybbt
    bbt_pollevent
    bbt_cxxlua
    bbt_network
)

if(NOT NO_EXAMPLE)
    add_subdirectory(example)
endif()


if(NOT NO_TEST)
    enable_testing()
    add_subdirectory(unit_test)    
endif()