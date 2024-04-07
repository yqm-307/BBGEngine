cmake_minimum_required(VERSION 3.2)

project(dbgClient)

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 -g ${Wall_Flag}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) # 可执行文件目录

set(PROJECT_SRC ${PROJECT_SOURCE_DIR}/../..)
set(COMMOM_SRC ${PROJECT_SOURCE_DIR}/../../src/common)
set(SERVER_SRC ${PROJECT_SOURCE_DIR}/../../src/server)
set(CLIENT_SRC ${PROJECT_SOURCE_DIR}/../../src/client)


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
    # engine module
    ${COMMOM_SRC}/*.cc
    # ecs define module
    ${SERVER_SRC}/share/*.cc
    ${PROJECT_SOURCE_DIR}/*.cc
)


# 为可执行程序添加CPP源文件
add_executable(dbgclient
    main.cc
    ${SRC_FILE}
)

# 为可执行程序添加链接库
target_link_libraries(dbgclient
    event_core
    event_pthreads
    bbt_network
    ybbt
    inih
    protobuf
    # tcmalloc
    dl
    ${LIB_BBGCOMM}
    lua
)