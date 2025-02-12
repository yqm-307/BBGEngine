#pragma once
#include "plugin/ecs/luascript/LuaScriptBaseComp.hpp"

namespace plugin::ecs::luascript
{

class LuaTestComp:
    public LuaScriptBaseComp
{
public:
    ComponentClassMetaInfo(EM_COMPONENT_TYPE_LUA_TEST);
    virtual ~LuaTestComp() {}
    bool Init(const char* script_path);
protected:
    LuaTestComp(bbt::cxxlua::LuaVM* vm);
};

}