cmake_minimum_required(VERSION 3.1)

project(my_mmo)

# 第三方引用的root路径
set(THIRD_PARTY_PATH ${PROJECT_SOURCE_DIR}/thirdparty)
set(THIRD_PARTY_LIB_PATH ${THIRD_PARTY_PATH}/libs)
set(THIRD_PARTY_INCLUDE_PATH ${THIRD_PARTY_PATH}/include)

add_subdirectory(unit_test)

add_subdirectory(gameserver)