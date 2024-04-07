#pragma once
#include "share/ecs/luascript/LuaScriptBaseComp.hpp"

namespace share::ecs::luascript
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