cmake_minimum_required(VERSION 3.1)

project(unittest)

enable_language(CXX)


set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 -g ${Wall_Flag}")

include_directories(
    ${PROJECT_SOURCE_DIR}/..        # util、share等库
    ${PROJECT_SOURCE_DIR}           # gs源文件
)

file(GLOB_RECURSE SRC_FILE
    ${PROJECT_SOURCE_DIR}/../share/*.cc
    ${PROJECT_SOURCE_DIR}/../util/*.cc
)

add_executable(unittest Test_Hashmap.cc ${SRC_FILE})
target_link_libraries(unittest
    boost_unit_test_framework
    boost_test_exec_monitor
    ybbt
)

add_executable(aoi_test Test_aoi.cc ${SRC_FILE})
target_link_libraries(aoi_test
    boost_unit_test_framework
    boost_test_exec_monitor
    ybbt
)

add_executable(aabb_test Test_aabbbox.cc ${SRC_FILE})
target_link_libraries(aabb_test
    boost_unit_test_framework
    boost_test_exec_monitor
    ybbt
)