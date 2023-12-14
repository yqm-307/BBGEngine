#pragma once
#include "share/ecs/aoi/entity/Define.hpp"
#include "util/config/Config.hpp"
#include "util/hashmap/Hashmap.hpp"
#include "util/vector/Pos3.hpp"
#include "share/ecs/aoi/component/AoiComponent.hpp"
#include <map>
#include <vector>

namespace share::ecs::entity::aoi
{

#pragma region "宏定义"

static const int AoiHashBucketNum = 16;
static const int AoiInfoIntervalMS = 5000;

#pragma endregion

/**
 * @brief Aoi是基于笛卡尔坐标系. 九宫格实现的
 * 
 */
class Aoi:
    public engine::ecs::GameObject
{
    GameObjectDeriveClassDef;
public:
    typedef std::vector<engine::ecs::GameObjectSPtr>     EntityResult;
    ~Aoi();
protected:
    Aoi();
    bool CheckConfig(const util::config::AoiConfig*) const;
    /**
     * @brief Aoi 驱动
     */
    virtual void OnUpdate() override;

    virtual void OnCreate() {}
    virtual void OnDestory() {}
    virtual void OnFatherDead() override {}

public:

    
private:
};

}// namespace share