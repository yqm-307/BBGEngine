#pragma once
#include <bbt/base/cxxlua/CXXLua.hpp>
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::globalluavm
{

class GlobalLuaVM:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;
    virtual ~GlobalLuaVM();

    bool PreloadLuaFile(const char* dir_path);
    void DoScript(const std::string& script);
private:
    GlobalLuaVM();
    virtual void OnUpdate() override {};

    bbt::cxxlua::LuaVM*     m_lua_vm{nullptr};
};

} // namespace share::ecs::globalluavm
