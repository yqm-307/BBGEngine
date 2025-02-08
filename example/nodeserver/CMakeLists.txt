set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/example/bin) # 可执行文件目录

# 包含头文件目录
include_directories(
    ${PROJECT_SOURCE_DIR}/example
    ${PROJECT_SOURCE_DIR}/thirdparty/include
    ${PROJECT_SOURCE_DIR}/src
)

# 搜索CPP文件
file(GLOB_RECURSE SRC_FILE
    # nodeserver 目录下
    ${PROJECT_SOURCE_DIR}/example/nodeserver/*.cc
    ${PROJECT_SOURCE_DIR}/../protocol
)

# 为可执行程序添加CPP源文件
add_executable(nodeserver
    main.cc
    ${SRC_FILE}
)

# 为可执行程序添加链接库
target_link_libraries(nodeserver PRIVATE
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
    BBGEngine
    lua
)