set(Wall_Flag "-Wall -Wno-sign-compare -Wno-format -Wno-reorder -Wno-unknown-pragmas")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti -std=c++17 ${Wall_Flag}")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin/debug)

# include目录
include_directories(
    ${PROJECT_SOURCE_DIR}/common                # util、share等库
    ${PROJECT_SOURCE_DIR}/share                 # util、share等库
    ${PROJECT_SOURCE_DIR}/server                # gs源文件
    ${THIRD_PARTY_INCLUDE_PATH}
)

# c++待编译的源文件
file(GLOB_RECURSE SRC_FILE
    ${PROJECT_SOURCE_DIR}/server/share/ecs/aoi/*.cc
    ${PROJECT_SOURCE_DIR}/server/share/ecs/gameobject/*.cc
    ${PROJECT_SOURCE_DIR}/server/share/ecs/testobj/*.cc
    ${PROJECT_SOURCE_DIR}/server/share/ecs/testobj/*.cc
    ${PROJECT_SOURCE_DIR}/server/share/scene/*.cc
)

message(${SRC_FILE})

set(MY_DBG_LIBS
    inih
    bbt_cxxlua
    ybbt
    ${PROJECT_BINARY_DIR}/common/lib/libBBGCommon.so
)

######################## ==> timewheel 测试 <== ########################
file(GLOB_RECURSE timewheel_component_src
    ${PROJECT_SOURCE_DIR}/server/share/ecs//timewheel/*.cc
)

add_executable(debug_timewheel
    ${SRC_FILE}
    ${timewheel_component_src}
    timewheel_test.cc
)

target_link_libraries(debug_timewheel
    ${MY_DBG_LIBS}
)