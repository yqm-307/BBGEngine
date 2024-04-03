#include <filesystem>
#include <cstring>
#include <regex>
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

    RegistInLuaEvent();
}

bool LuaScriptComponent::IsInitSucc()
{
    return m_init_success;
}

LuaScriptComponent::~LuaScriptComponent()
{
    UnRegistInLuaEvent();
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
    std::string path = std::regex_replace(std::regex_replace(m_script_path, std::regex(".lua"), ""), std::regex("//"), ".");

    //TODO 等待cxxlua完善lua读取栈数据操作
    auto err = m_vm->CallLuaFunction("CppCallRegist", 2, [](std::unique_ptr<bbt::cxxlua::detail::LuaStack>& stack)->std::optional<bbt::cxxlua::LuaErr>{
        bbt::cxxlua::Value msg;
        bbt::cxxlua::Value bsucc;
        auto pop_msg_err = stack->Pop(msg);
        if (pop_msg_err != std::nullopt)
            return pop_msg_err;
        
        auto pop_bsucc_err = stack->Pop(bsucc);
            return pop_bsucc_err;

        return std::nullopt;
    }, GetMemberId(), path.c_str());

    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
        return;
    }
}

/* 反注册到lua事件 */
void LuaScriptComponent::UnRegistInLuaEvent()
{

}

void LuaScriptComponent::OnUpdate()
{
    auto err = m_vm->DoScript("ComponentMgr:OnUpdateEvent(" + std::to_string(GetMemberId()) + ")");
    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
    }
}


} // namespace share::ecs::luascript


#ifdef SCRIPT_BUFFER_SIZE
#undef SCRIPT_BUFFER_SIZE
#endif