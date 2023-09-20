#pragma once
#include "util/hashmap/Hashmap.hpp"
#include <bbt/templateutil/BaseType.hpp>
#include <optional>



namespace game::share::ecs
{
///////////////////////////////////////////////////////////////////
#pragma region "组件名"

struct _Global_Static_ComponentName
{
    const std::string component_none_name = "";
    const std::string component_aoi_name = "AoiComponent";
};

/* 所有组件的name，生命期是global、static的  */
static const _Global_Static_ComponentName GSComponentName;



#pragma endregion

///////////////////////////////////////////////////////////////////


enum ComponentTemplateId : int32_t
{
    EM_NoneComponent    =   0,
    EM_AoiComponent     =   10001,
};



struct ComponentInfo:
    public bbt::templateutil::BaseType<ComponentInfo>
{
    ComponentInfo() = default;
    ComponentInfo(const std::string& name):Name(name){}
    const std::string& Name;
};

}


