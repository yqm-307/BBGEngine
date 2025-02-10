#pragma once
#include "plugin/scene/basescene/BaseScene.hpp"

namespace plugin::scene
{

/**
 * @brief 地图场景
 * 应该包含基础的地图信息、玩家信息、npc信息以及出入地图的回调接口等 
 */
class MapScene:
    public plugin::scene::BaseScene
{
};

}