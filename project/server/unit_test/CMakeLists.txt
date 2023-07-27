cmake_minimum_required(VERSION 3.1)

project(unittest)

enable_language(CXX)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 -O3 -g ${Wall_Flag}")

include_directories(
    ${PROJECT_SOURCE_DIR}/..        # util、share等库
    ${PROJECT_SOURCE_DIR}           # gs源文件
)

file(GLOB_RECURSE SRC_FILE
    ${PROJECT_SOURCE_DIR}/*.cc
    ${PROJECT_SOURCE_DIR}/../share/*.cc
    ${PROJECT_SOURCE_DIR}/../util/*.cc
)

add_executable(unittest ${SRC_FILE})
target_link_libraries(unittest
    boost_unit_test_framework
    boost_test_exec_monitor
    ybbt
)
