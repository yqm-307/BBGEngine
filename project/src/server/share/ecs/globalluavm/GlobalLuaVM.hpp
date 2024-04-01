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
    bool DoScript(const std::string& script);
    bool LoadLuaLibrary();
    bool AddRequirePath(const std::string& path);

    template<typename ... Args>
    bool CallLuaFunction()
private:
    GlobalLuaVM();
    virtual void OnUpdate() override {};

    bbt::cxxlua::LuaVM*     m_lua_vm{nullptr};
};

} // namespace share::ecs::globalluavm
