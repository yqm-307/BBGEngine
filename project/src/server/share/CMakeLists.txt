cmake_minimum_required(VERSION 3.2)

add_definitions(-DDebug)

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=c++17 -g ${Wall_Flag}")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/common/lib) # 库文件目录

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    ${PROJECT_SOURCE_DIR}/../thirdparty/libs
    ${PROJECT_BINARY_DIR}/common/lib
)

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/server
)

file(GLOB_RECURSE SRC_FILE
    # share 目录下
    ${PROJECT_SOURCE_DIR}/server/share/*.cc
)

add_library(BBGShare SHARED ${SRC_FILE})

target_link_libraries(BBGShare
    ybbt
    bbt_pollevent
    bbt_cxxlua
    bbt_network
    BBGCommon
)