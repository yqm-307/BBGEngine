
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/unit_test)

include_directories(
    ${PROJECT_SOURCE_DIR}/src
)


# 链接库
set(MY_LINK_LIB
    boost_unit_test_framework
    boost_test_exec_monitor
    event_core
    bbt_cxxlua
    bbt_core
    pthread
    dl
    BBGEngine
    lua
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


add_executable(Test_scene engine/Test_scene.cc)
target_link_libraries(Test_scene ${MY_LINK_LIB})
add_test(NAME Test_scene COMMAND Test_scene)