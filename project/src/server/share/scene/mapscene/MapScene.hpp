#pragma once
#include "share/scene/basescene/BaseScene.hpp"

namespace share::scene
{

/**
 * @brief 地图场景
 * 应该包含基础的地图信息、玩家信息、npc信息以及出入地图的回调接口等 
 */
class MapScene:
    public share::scene::BaseScene
{
};

}