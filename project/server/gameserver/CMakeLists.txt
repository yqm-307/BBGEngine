cmake_minimum_required(VERSION 3.2)

# project(gameserver)

enable_language(CXX)

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 -g ${Wall_Flag}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin) # 可执行文件目录
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib) # 库输出路径

include_directories(
    ${PROJECT_SOURCE_DIR}/..        # util、share等库
    ${PROJECT_SOURCE_DIR}           # gs源文件
)
# 搜索CPP文件
file(GLOB_RECURSE SRC_FILE
    # gameserver 目录下
    ${PROJECT_SOURCE_DIR}/gameserver/*.cc
    # share 目录下
    ${PROJECT_SOURCE_DIR}/share/*.cc
    # util 目录下
    ${PROJECT_SOURCE_DIR}/util/*.cc
)

# 为可执行程序添加CPP源文件
add_executable( gameserver 
    main.cc
    ${SRC_FILE}
)


# 为可执行程序添加链接库
target_link_libraries( gameserver
    pthread
    ybbt
)