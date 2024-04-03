#pragma once
#include <bbt/base/cxxlua/CXXLua.hpp>
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::luascript
{

class LuaScriptComponent:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;
    virtual ~LuaScriptComponent();

    bool    IsInitSucc();
    std::optional<bbt::cxxlua::LuaErr> DoScript(const char* script);
private:
    virtual void OnUpdate() override;
    LuaScriptComponent(bbt::cxxlua::LuaVM* vm, const char* relative_path);
    /* 注册到lua事件 */
    void RegistInLuaEvent();
    /* 反注册到lua事件 */
    void UnRegistInLuaEvent();

    std::string         m_script_path{""};      // 挂载的脚本路径
    bool                m_init_success{false};  // 是否初始化成功
    bbt::cxxlua::LuaVM* m_vm{nullptr};          // 脚本运行的虚拟机
};

} // namespace share::ecs::luascript