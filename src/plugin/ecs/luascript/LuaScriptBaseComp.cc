#include <filesystem>
#include <cstring>
#include <regex>
#include <bbt/cxxlua/detail/LuaErr.hpp>
#include "plugin/ecs/luascript/LuaScriptBaseComp.hpp"

namespace share::ecs::luascript
{


LuaScriptBaseComp::LuaScriptBaseComp(ecs::emComponentType id, bbt::cxxlua::LuaVM* vm):
    Component(id),
    m_vm(vm)
{}

LuaScriptBaseComp::~LuaScriptBaseComp()
{
    UnRegistInLuaEvent();
}

std::optional<bbt::cxxlua::LuaErr> LuaScriptBaseComp::DoScript(const char* script)
{
    if (m_vm == nullptr)
        return bbt::cxxlua::LuaErr("vm is NULL!", bbt::cxxlua::ERRCODE::Default);
    
    return m_vm->DoScript(script);
}

/* 注册到lua事件 */
void LuaScriptBaseComp::RegistInLuaEvent()
{
    std::string path = std::regex_replace(std::regex_replace(m_script_path, std::regex(".lua"), ""), std::regex("//"), ".");

    //TODO 等待cxxlua完善lua读取栈数据操作
    auto err = m_vm->CallLuaFunction("CppCallRegist", 2, [](std::shared_ptr<bbt::cxxlua::detail::LuaStack>& stack)->std::optional<bbt::cxxlua::LuaErr>{
        auto [get_msg_err, msg_opt] = stack->GetValue(-1);
        if (get_msg_err)
            return get_msg_err;

        auto [get_succ_err, bsucc_opt] = stack->GetValue(-2);
        if (get_succ_err)
            return get_succ_err;

        return std::nullopt;
    }, GetMemberId(), path.c_str());

    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
        return;
    }
}

/* 反注册到lua事件 */
void LuaScriptBaseComp::UnRegistInLuaEvent()
{

}

void LuaScriptBaseComp::OnUpdate()
{
    _SendLuaComponentMgrUpdateEvent();
}

void LuaScriptBaseComp::_SendLuaComponentMgrUpdateEvent()
{
    auto err = m_vm->DoScript("ComponentMgr:OnUpdateEvent(" + std::to_string(GetMemberId()) + ")");
    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR("DoScript() error! %s", err.value().What().c_str());
    }
}

bool LuaScriptBaseComp::RegistScript(const char* relative_path)
{
    m_script_path = relative_path;

    if (!std::filesystem::exists(m_script_path)) {
        GAME_EXT1_LOG_ERROR("[%s] script file not exist!", m_script_path);
        return false;
    }

    auto err = m_vm->LoadFile(m_script_path);
    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
        return false;
    }

    RegistInLuaEvent();

    return true;
}

std::string& LuaScriptBaseComp::GetScriptPath()
{
    return m_script_path;
}



} // namespace share::ecs::luascript
