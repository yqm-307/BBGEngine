#include "share/ecs/ComponentDef.hpp"

namespace game::share::ecs
{


std::string GetComponentName(ComponentTemplateId id)
{
    return GetComponentInfo(id).Name;
}

ComponentInfo GetComponentInfo(ComponentTemplateId id)
{
    auto [val, isexist] = ComponentInfoMap.Find(id);
    if(isexist)
        return *val;
    
    return ComponentInfo();
}


}