#include "plugin/ecs/globalluavm/GlobalLuaVM.hpp"


namespace share::ecs::globalluavm
{

GlobalLuaVM::~GlobalLuaVM()
{
}

GlobalLuaVM::GlobalLuaVM():
    engine::ecs::Component(EM_COMPONENT_TYPE_GLOBAL_LUA_VM),
    m_lua_vm(new bbt::cxxlua::LuaVM())
{
    auto err = m_lua_vm->LoadLuaLibrary();
    if (err != std::nullopt)
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());

    auto err1 = m_lua_vm->DoScript("collectgarbage(\"generational\")");
    if (err1 != std::nullopt)
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());

}


bool GlobalLuaVM::PreloadLuaFile(const char* dir_path)
{
    auto err = m_lua_vm->LoadFolder(dir_path);
    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
        return false;
    }

    return true;
}

bool GlobalLuaVM::DoScript(const std::string& script)
{
    auto err = m_lua_vm->DoScript(script);
    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
        return false;
    }

    return true;
}

bool GlobalLuaVM::LoadLuaLibrary()
{
    auto err = m_lua_vm->LoadLuaLibrary();
    if (err != std::nullopt) {
        GAME_EXT1_LOG_ERROR(err.value().What().c_str());
        return false;
    }

    return true;
}

bool GlobalLuaVM::AddRequirePath(const std::string& path)
{
    return DoScript("package.path = \";" + path + ";\" .. package.path");
}

bbt::cxxlua::LuaVM* GlobalLuaVM::GetVM()
{
    return m_lua_vm;
}


} // namespace share::ecs::globalluavm
