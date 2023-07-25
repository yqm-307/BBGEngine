#pragma once
#include <map>
#include "share/aoi/Define.hpp"

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
    size_t      m_length;
    MapSlot*    m_slots;
};

}