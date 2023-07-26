#pragma once
#include <map>
#include <vector>
#include "share/aoi/Define.hpp"
#include "util/config/config.hpp"
#include "util/vector/Pos3.hpp"

namespace game::share::aoi
{
/**
 * @brief Aoi是基于笛卡尔坐标系. 九宫格实现的
 * 
 */

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
    /**
     * AOI 相关
     */
    Tower* GetTowerByIndex3(util::pos::Index3 index3);
    util::pos::Index3 GetIndex3ByIndex(int tower_index);
private:
    size_t      m_length;
    std::vector<MapSlot>   m_slots;    // AOI中所有Slot
    std::vector<Tower>     m_towers;   // AOI中所有灯塔
    int m_tower_max_x;
    int m_tower_max_y;
    int m_tower_max_z;
    const util::config::AoiConfig*      m_config;   // AOI配置
    std::map<AoiObjectId, game::share::ecs::GameObject::SPtr>   m_gameobj_map;  // AOI中所有游戏对象

};

}