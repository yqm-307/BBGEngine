#pragma once
#include <bbt/cxxlua/CXXLua.hpp>
#include "engine/ecs/component/Component.hpp"
#include "plugin/ecs/Define.hpp"

namespace plugin::ecs::globalluavm
{

class GlobalLuaVM:
    public engine::ecs::Component
{
public:
    typedef bbt::cxxlua::detail::LuaParseReturnCallback ParseLuaRetValFunc;
    ComponentDeriveClassDef;
    virtual ~GlobalLuaVM();

    bool PreloadLuaFile(const char* dir_path);
    bool DoScript(const std::string& script);
    bool LoadLuaLibrary();
    bool AddRequirePath(const std::string& path);

    template<typename ... Args>
    bool CallLuaFunction(
        const std::string&  funcname,
        int                 return_nums,
        const ParseLuaRetValFunc& parse_handle,
        Args                ...args)
    {
        auto err = m_lua_vm->CallLuaFunction(funcname, return_nums, parse_handle, args ...);
        if (err != std::nullopt) {
            GAME_EXT1_LOG_ERROR(err.value().What().c_str());
            return false;
        }

        return true;
    }

    bbt::cxxlua::LuaVM* GetVM();
private:
    GlobalLuaVM();
    virtual void OnUpdate() override {};

    bbt::cxxlua::LuaVM*     m_lua_vm{nullptr};
};

} // namespace share::ecs::globalluavm
