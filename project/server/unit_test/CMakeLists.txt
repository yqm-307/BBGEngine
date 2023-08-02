cmake_minimum_required(VERSION 3.1)

# project(unittest)

enable_language(CXX)


set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 ${Wall_Flag}")

# debug版本
add_definitions(-DDebug)

# include目录
include_directories(
    ${PROJECT_SOURCE_DIR}/util        # util、share等库
    ${PROJECT_SOURCE_DIR}/share        # util、share等库
    ${PROJECT_SOURCE_DIR}           # gs源文件
    ${THIRD_PARTY_PATH}/include
)

# lib文件搜索目录
link_directories(${THIRD_PARTY_PATH}/libs)

# c++待编译的源文件
file(GLOB_RECURSE SRC_FILE
    ${PROJECT_SOURCE_DIR}/share/*.cc
    ${PROJECT_SOURCE_DIR}/util/*.cc
)

# 链接库
set(MY_LINK_LIB
    boost_unit_test_framework
    boost_test_exec_monitor
    ybbt
    inih
)

add_executable(unittest Test_Hashmap.cc ${SRC_FILE})
target_link_libraries(unittest
    ${MY_LINK_LIB}
)

add_executable(aoi_test Test_aoi.cc ${SRC_FILE})
target_link_libraries(aoi_test
    ${MY_LINK_LIB}
)

add_executable(aabb_test Test_aabbbox.cc ${SRC_FILE})
target_link_libraries(aabb_test
    ${MY_LINK_LIB}
)