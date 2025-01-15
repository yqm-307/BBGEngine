cmake_minimum_required(VERSION 3.2)

project(BBGCommon CXX)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O2")

set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH}
    ${PROJECT_SOURCE_DIR}/../../thirdparty/libs
)
include_directories(
    ${PROJECT_SOURCE_DIR}      # util、share等库
    ${PROJECT_SOURCE_DIR}/../../thirdparty/include
)


file(GLOB_RECURSE SRCS 
    ${PROJECT_SOURCE_DIR}/*.cc
)

add_library(BBGCommon SHARED ${SRCS})

target_link_libraries(BBGCommon
    ybbt
    inih
    protobuf
)