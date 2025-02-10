#pragma once
#include "plugin/ecs/luascript/LuaScriptBaseComp.hpp"

namespace plugin::ecs::luascript
{

class LuaTestComp:
    public LuaScriptBaseComp
{
public:
    ComponentDeriveClassDef;
    virtual ~LuaTestComp() {}
    bool Init(const char* script_path);
protected:
    LuaTestComp(bbt::cxxlua::LuaVM* vm);
};

}