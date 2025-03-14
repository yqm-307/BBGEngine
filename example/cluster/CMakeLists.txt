set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/example/bin) # 可执行文件目录

# 为可执行程序添加CPP源文件
add_executable(cluster_registery
    ${PROJECT_SOURCE_DIR}/example/cluster/registery.cc
)

# 为可执行程序添加链接库
target_link_libraries(cluster_registery PRIVATE
    event_core
    event_pthreads
    bbt_pollevent
    bbt_cxxlua
    bbt_network
    bbt_core
    inih
    protobuf
    # tcmalloc
    dl
    BBGEngine
    lua
)

add_executable(cluster_node
    ${PROJECT_SOURCE_DIR}/example/cluster/service.cc
)

target_link_libraries(cluster_node PRIVATE
    event_core
    event_pthreads
    bbt_pollevent
    bbt_cxxlua
    bbt_network
    bbt_core
    inih
    protobuf
    # tcmalloc
    dl
    BBGEngine
    lua
)

add_executable(rpc_client
    ${PROJECT_SOURCE_DIR}/example/cluster/client.cc
)

target_link_libraries(rpc_client PRIVATE
    event_core
    event_pthreads
    bbt_pollevent
    bbt_cxxlua
    bbt_network
    bbt_core
    inih
    protobuf
    # tcmalloc
    dl
    BBGEngine
    lua
)