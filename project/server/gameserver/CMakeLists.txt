cmake_minimum_required(VERSION 3.2)



enable_language(CXX)

add_definitions(-DDebug)

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 -g ${Wall_Flag}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) # 可执行文件目录

set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} ./thirdparty/libs)

include_directories(
    ${PROJECT_SOURCE_DIR}/util        # util、share等库
    ${PROJECT_SOURCE_DIR}/share       # util、share等库
    ${PROJECT_SOURCE_DIR}/engine      # 引擎
    ${PROJECT_SOURCE_DIR}           # gs源文件
    ${THIRD_PARTY_INCLUDE_PATH}
)


# 搜索CPP文件
file(GLOB_RECURSE SRC_FILE
    # gameserver 目录下
    ${PROJECT_SOURCE_DIR}/gameserver/*.cc
    # share 目录下
    ${PROJECT_SOURCE_DIR}/share/*.cc
    # engine 目录下
    ${PROJECT_SOURCE_DIR}/engine/*.cc
    # util 目录下
    ${PROJECT_SOURCE_DIR}/util/*.cc
)

# 为可执行程序添加CPP源文件
add_executable(gameserver 
    main.cc
    ${SRC_FILE}
)

# 为可执行程序添加链接库
target_link_libraries(gameserver
    event_core
    pthread
    ybbt
    inih
    protobuf
    tcmalloc
)