cmake_minimum_required(VERSION 3.2)

add_definitions(-DDebug)

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g ${Wall_Flag}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) # 可执行文件目录

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    ${PROJECT_SOURCE_DIR}/../thirdparty/libs
    ${PROJECT_BINARY_DIR}/common/lib
)

include_directories(
    ${PROJECT_SOURCE_DIR}/common
    ${PROJECT_SOURCE_DIR}/server
    ${PROJECT_SOURCE_DIR}
)
# 搜索CPP文件
file(GLOB_RECURSE SRC_FILE
    # nodeserver 目录下
    ${PROJECT_SOURCE_DIR}/server/nodeserver/*.cc
)


# 为可执行程序添加CPP源文件
add_executable(nodeserver
    main.cc
    ${SRC_FILE}
)

# 为可执行程序添加链接库
target_link_libraries(nodeserver
    event_core
    event_pthreads
    bbt_pollevent
    bbt_cxxlua
    bbt_network
    ybbt
    inih
    protobuf
    # tcmalloc
    dl
    BBGCommon
    BBGShare
    lua
)