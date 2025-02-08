cmake_minimum_required(VERSION 3.2)

project(dbServiceRobot)

add_definitions("-DDebug")

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g ${Wall_Flag}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) # 可执行文件目录

set(PROJECT_SRC ${PROJECT_SOURCE_DIR}/../..)
set(COMMOM_SRC ${PROJECT_SOURCE_DIR}/../../src/common)
set(SERVER_SRC ${PROJECT_SOURCE_DIR}/../../src/server)


set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} 
    ${PROJECT_SRC}/thirdparty/libs
    ${PROJECT_SRC}/server/build/common/lib
)

find_library(LIB_BBGCOMM BBGCommon ${PROJECT_SRC}/server/build/common/lib/BBGCommon)

include_directories(
    ${PROJECT_SOURCE_DIR}/../../src/common
    ${PROJECT_SOURCE_DIR}/../../src/server
    ${PROJECT_SRC}/thirdparty/include
)

# 搜索CPP文件
file(GLOB_RECURSE SRC_FILE
    # ecs define module
    ${SERVER_SRC}/share/ecs/network/*.cc
    ${SERVER_SRC}/share/ecs/gameobject/*.cc
    ${SERVER_SRC}/share/ecs/timewheel/*.cc
    ${SERVER_SRC}/share/session/*.cc
    ${PROJECT_SOURCE_DIR}/*.cc
)

# 为可执行程序添加CPP源文件
add_executable(dbservrobot
    main.cc
    ${SRC_FILE}
)

# 为可执行程序添加链接库
target_link_libraries(dbservrobot
    ${LIB_BBGCOMM}
    ybbt
    event_core
    event_pthreads
    bbt_network
    protobuf
    # tcmalloc
    dl
    lua
)