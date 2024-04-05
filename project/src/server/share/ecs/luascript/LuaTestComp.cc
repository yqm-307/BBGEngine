#include "share/ecs/luascript/LuaTestComp.hpp"



namespace share::ecs::luascript
{

LuaTestComp::LuaTestComp(bbt::cxxlua::LuaVM* vm, const char* script_path):
    LuaScriptBaseComp(EM_COMPONENT_TYPE_LUA_TEST, vm)
{
    m_init_succ = RegistScript(script_path);
}



}
