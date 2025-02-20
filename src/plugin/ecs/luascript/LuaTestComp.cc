#include "plugin/ecs/luascript/LuaTestComp.hpp"



namespace plugin::ecs::luascript
{

LuaTestComp::LuaTestComp(bbt::cxxlua::LuaVM* vm):
    LuaScriptBaseComp(vm)
{
}

bool LuaTestComp::Init(const char* script_path)
{
    return RegistScript(script_path);
}


}
