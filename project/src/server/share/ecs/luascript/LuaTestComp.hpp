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
    bool IsInitSucc() { return m_init_succ; }
protected:
    LuaTestComp(bbt::cxxlua::LuaVM* vm, const char* script_path);

    bool m_init_succ{false};
};

}