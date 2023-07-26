#pragma once
#include <map>
#include <vector>
#include "share/aoi/Define.hpp"
#include "util/config/config.hpp"

namespace game::share::aoi
{

class Aoi:
    public bbt::templateutil::BaseType<Aoi>
{
public:
    static RawPtr GetInstance();
private:
    Aoi();
    ~Aoi();

    void Init();
    void OnEnter(game::share::ecs::GameObject::SPtr player);
    void OnLeave(game::share::ecs::GameObject::SPtr player);
    void OnMove(game::share::ecs::GameObject::SPtr player);
    void OnUpdate(game::share::ecs::GameObject::SPtr player);
private:
    bool CheckConfig(const util::config::AoiConfig*);
private:
    size_t      m_length;
    std::vector<MapSlot>   m_slots;    // AOI中所有Slot
    std::vector<Tower>     m_towers;   // AOI中所有灯塔
    const util::config::AoiConfig*      m_config;   // AOI配置
    std::map<AoiObjectId, game::share::ecs::GameObject::SPtr>   m_gameobj_map;  // AOI中所有游戏对象

};

}