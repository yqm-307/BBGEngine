cmake_minimum_required(VERSION 3.2)

project(top)

enable_testing()
# 公用程序
add_subdirectory(common)

# 服务端程序
add_subdirectory(server)

# 客户端 dll
add_subdirectory(client)