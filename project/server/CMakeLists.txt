cmake_minimum_required(VERSION 3.2)

project(game)

include_directories(
    ${PROJECT_SOURCE_DIR}/server
    /usr/local/include
    /usr/include
)
message(${PROJECT_SOURCE_DIR}/server)