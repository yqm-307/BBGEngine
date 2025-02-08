#include "plugin/ecs/luascript/LuaTestComp.hpp"



namespace share::ecs::luascript
{

LuaTestComp::LuaTestComp(bbt::cxxlua::LuaVM* vm):
    LuaScriptBaseComp(EM_COMPONENT_TYPE_LUA_TEST, vm)
{
}

bool LuaTestComp::Init(const char* script_path)
{
    return RegistScript(script_path);
}


}
