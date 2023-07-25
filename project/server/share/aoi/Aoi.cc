#include "share/aoi/Aoi.hpp"
#include "util/config/config.hpp"
#include "util/log/log.hpp"

using namespace game::share::aoi;


Aoi::RawPtr Aoi::GetInstance()
{
    static RawPtr instance = nullptr;
    if(instance == nullptr)
    {
        instance = new Aoi();
    }
    return instance;
}

Aoi::Aoi()
{
    Init();
}

Aoi::~Aoi()
{

}

void Aoi::Init()
{
    auto* config = G_GetConfigPtr(util::config::AoiConfig, util::config::Cfg_Aoi);
    if(config == nullptr){
        ERROR("[Aoi::Init] config not found!");
    }

}

void Aoi::OnEnter(game::share::ecs::GameObject::SPtr player)
{

}

void Aoi::OnLeave(game::share::ecs::GameObject::SPtr player)
{

}

void Aoi::OnMove(game::share::ecs::GameObject::SPtr player)
{

}

void Aoi::OnUpdate(game::share::ecs::GameObject::SPtr player)
{

}