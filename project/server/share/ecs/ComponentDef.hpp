#pragma once
#include "util/hashmap/Hashmap.hpp"
#include <bbt/templateutil/BaseType.hpp>
#include <optional>



namespace game::share::ecs
{
///////////////////////////////////////////////////////////////////
#pragma region "组件名"

inline const std::string component_aoi_name = "AoiComponent";

#pragma endregion

///////////////////////////////////////////////////////////////////


enum ComponentTemplateId : int32_t
{
    EM_AoiComponent    =   10001,
};



struct ComponentInfo:
    public bbt::templateutil::BaseType<ComponentInfo>
{
    ComponentInfo() = default;
    ComponentInfo(std::string name):Name(name){}
    std::string Name{""};
};

}


