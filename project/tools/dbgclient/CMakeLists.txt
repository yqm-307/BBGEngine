cmake_minimum_required(VERSION 3.1)

project(my_mmo)

# 搜索CPP文件
file(GLOB_RECURSE SRC_FILE
    ${PROJECT_SOURCE_DIR}/*.cc
)

# 为可执行程序添加CPP源文件
add_executable(dbgclient 
    main.cc
    ${SRC_FILE}
)

# 为可执行程序添加链接库
target_link_libraries(gameserver
    # event_core
    event_pthreads
    bbt_network
    ybbt
    # inih
    # protobuf
    # tcmalloc
    # dl
)