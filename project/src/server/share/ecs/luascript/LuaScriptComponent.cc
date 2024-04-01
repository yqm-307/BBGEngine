#include <filesystem>
#include <cstring>
#include <bbt/base/cxxlua/detail/LuaErr.hpp>
#include "share/ecs/luascript/LuaScriptComponent.hpp"

namespace share::ecs::luascript
{

const char* g_regist_in_lua_event = R"(
local _ComponentMgr = require("preload.ComponentMgr")
local _Tools        = require("preload.Tools")
local _TargetScript = require("%s")

if not _TargetScript.OnUpdate then
    _Tools.LogErr("请实现 OnUpdate 函数")
end

_ComponentMgr:RegistComponent("%d", _TargetScript.OnUpdate)
)";

#define SCRIPT_BUFFER_SIZE 4096

LuaScriptComponent::LuaScriptComponent(bbt::cxxlua::LuaVM* vm, const char* relative_path):
    Component(ecs::EM_COMPONENT_TYPE_LUA_SCRIPT),
    m_vm(vm),
    m_script_path(relative_path)
{
    if (!std::filesystem::exists(m_script_path)) {
        return;
    }

    if (m_vm == nullptr) {
        return;
    }

    auto err = m_vm->LoadFile(m_script_path);
    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR("%s", err.value().What().c_str());
        return;
    }

    m_init_success = true;
}

bool LuaScriptComponent::IsInitSucc()
{
    return m_init_success;
}

LuaScriptComponent::~LuaScriptComponent()
{
}

std::optional<bbt::cxxlua::LuaErr> LuaScriptComponent::DoScript(const char* script)
{
    if (m_vm == nullptr)
        return bbt::cxxlua::LuaErr("vm is NULL!", bbt::cxxlua::ERRCODE::Default);
    
    return m_vm->DoScript(script);
}


/* 注册到lua事件 */
void LuaScriptComponent::RegistInLuaEvent()
{
    static char script[SCRIPT_BUFFER_SIZE] = "";
    std::string path = m_script_path;
    int total_size = 0;

    total_size = m_script_path.size() + sizeof(*g_regist_in_lua_event) + 20;/* ComponentId 最大值字符串长度 */
    if (total_size > SCRIPT_BUFFER_SIZE) {
        GAME_EXT1_LOG_ERROR("lua script buffer overflow!");
        return;
    }
    memset(script, '\0', SCRIPT_BUFFER_SIZE);
    snprintf(script, SCRIPT_BUFFER_SIZE, g_regist_in_lua_event, m_script_path.c_str(), std::to_string(GetId()));

    GAME_EXT1_LOG_DEBUG(script);
    
    auto err = m_vm->DoScript(script);
    if (err != std::nullopt)
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
}

/* 反注册到lua事件 */
void LuaScriptComponent::UnRegistInLuaEvent()
{

}


} // namespace share::ecs::luascript


#ifdef SCRIPT_BUFFER_SIZE
#undef SCRIPT_BUFFER_SIZE
#endif