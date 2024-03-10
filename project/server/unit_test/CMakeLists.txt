cmake_minimum_required(VERSION 3.1)

# project(unittest)

enable_language(CXX)

set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 ${Wall_Flag}")
enable_testing()

# debug版本
add_definitions(-DDebug)

# include目录
include_directories(
    ${PROJECT_SOURCE_DIR}/util          # util、share等库
    ${PROJECT_SOURCE_DIR}/share         # util、share等库
    ${PROJECT_SOURCE_DIR}/engine        # util、share等库
    ${PROJECT_SOURCE_DIR}               # gs源文件
    ${THIRD_PARTY_INCLUDE_PATH}
)
    
# c++待编译的源文件
file(GLOB_RECURSE SRC_FILE
    ${PROJECT_SOURCE_DIR}/share/ecs/aoi/*.cc
    ${PROJECT_SOURCE_DIR}/share/ecs/gameobject/*.cc
    ${PROJECT_SOURCE_DIR}/share/ecs/testobj/*.cc
    ${PROJECT_SOURCE_DIR}/share/ecs/testobj/*.cc
    ${PROJECT_SOURCE_DIR}/share/scene/*.cc
    ${PROJECT_SOURCE_DIR}/engine/*.cc
    ${PROJECT_SOURCE_DIR}/util/*.cc
    # ${PROJECT_SOURCE_DIR}/unit_test/*.cc
)

# 链接库
set(MY_LINK_LIB
    boost_unit_test_framework
    boost_test_exec_monitor
    event_core
    ybbt
    inih
    pthread
    dl
)

##########################################################################
##############             测试案例编写cmake文件
##
##  命名规则：  test_[测试大类]_[测试小类(可选)]_texe
##
##  如:  测试 engine 的 scene 的功能命名就是 test_engine_scene_texe
##  如:  测试 util 的 random 就是 test_util_random_texe
##  如:  测试 hello world 就是 test_helloworld_texe
##
##
##
##########################################################################




######################## ==> 单元测试，后续需要拆分 <== ########################
add_executable(unit_test 
    ${SRC_FILE}
    ./main.cc
)
target_link_libraries(unit_test
    ${MY_LINK_LIB}
)
add_test(NAME unit_test COMMAND unit_test)


######################## ==> game 的 aoi 功能测试 <== ########################
add_executable(test_game_aoi_texe
    ${SRC_FILE}
    ./game/aoi/Test_aoi.cc
)

target_link_libraries(test_game_aoi_texe
    ${MY_LINK_LIB}
)
add_test(NAME test_game_aoi_texe COMMAND test_game_aoi_texe)

######################## ==> engine 的 scene 功能测试 <== ########################

add_executable(test_engine_scene_texe
    ${SRC_FILE}
    ./engine_test/Test_scene.cc
)

target_link_libraries(test_engine_scene_texe
    ${MY_LINK_LIB}
)
add_test(NAME engine_scene COMMAND test_engine_scene_texe)

######################## ==> engine 的 component mgr 功能测试 <== ########################

add_executable(test_engine_componentmgr_texe
    ${SRC_FILE}
    ./engine_test/Test_ecs_componentmgr.cc
)

target_link_libraries(test_engine_componentmgr_texe
    ${MY_LINK_LIB}
)

add_test(NAME engine_scene_component_mgr COMMAND test_engine_componentmgr_texe)


######################## ==> yqm 的 cpp with lua 测试 <== ########################

add_executable(test_cpp2lua_texe
    ${SRC_FILE}
    ./lua_test/cpp2lua.cc
)

target_link_libraries(test_cpp2lua_texe
    ${MY_LINK_LIB}
    lua
)

add_test(NAME test_cpp2lua_texe COMMAND test_cpp2lua_texe)

######################## ==> yqm 的 test helper debug 测试 <== ########################

add_executable(test_testhelperdebug_texe
    ${SRC_FILE}
    ./util_test/testhelper/testhelperdbg.cc
)

target_link_libraries(test_testhelperdebug_texe
    ${MY_LINK_LIB}
)

add_test(NAME test_testhelperdebug_texe COMMAND test_testhelperdebug_texe)

######################## ==> yqm 的 test helper debug 测试 <== ########################

add_executable(test_PointAndVector_texe
    ${SRC_FILE}
    ./util_test/vector/PointAndVectorTest.cc
)

target_link_libraries(test_PointAndVector_texe
    ${MY_LINK_LIB}
)

add_test(NAME test_PointAndVector_texe COMMAND test_PointAndVector_texe)

######################## ==> engine 的 gameobject debug 测试 <== ########################

add_executable(test_ecs_gameobjmgr_texe
    ${SRC_FILE}
    ./engine_test/Test_ecs_gameobjmgr.cc
)

target_link_libraries(test_ecs_gameobjmgr_texe
    ${MY_LINK_LIB}
)

add_test(NAME test_ecs_gameobjmgr_texe COMMAND test_ecs_gameobjmgr_texe)