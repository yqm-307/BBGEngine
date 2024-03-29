#include <filesystem>
#include <bbt/base/cxxlua/detail/LuaErr.hpp>
#include "share/ecs/luascript/LuaScriptComponent.hpp"

namespace share::ecs::luascript
{

LuaScriptComponent::LuaScriptComponent(bbt::cxxlua::LuaVM* vm, const char* relative_path):
    Component(ecs::EM_COMPONENT_TYPE_LUA_SCRIPT),
    m_vm(vm),
    m_script_path(relative_path)
{
    if (std::filesystem::exists(m_script_path)) {
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



} // namespace share::ecs::luascript
