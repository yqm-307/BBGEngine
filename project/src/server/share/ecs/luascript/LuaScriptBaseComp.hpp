#pragma once
#include <bbt/base/cxxlua/CXXLua.hpp>
#include "engine/ecs/component/Component.hpp"
#include "share/ecs/Define.hpp"

namespace share::ecs::luascript
{

class LuaScriptBaseComp:
    public engine::ecs::Component
{
public:
    ComponentDeriveClassDef;
    virtual ~LuaScriptBaseComp() = 0;

    /**
     * @brief 执行脚本指令
     * 
     * @param script 
     * @return std::optional<bbt::cxxlua::LuaErr> 
     */
    std::optional<bbt::cxxlua::LuaErr>  DoScript(const char* script);

    /**
     * @brief 加载lua script文件
     * 
     * @param relative_path 
     * @return true 
     * @return false 
     */
    bool                                RegistScript(const char* relative_path);

    /**
     * @brief 获取当前组件注册脚本的地址
     * 
     * @return std::string& 
     */
    std::string&                        GetScriptPath();

    /**
     * @brief 获取全局变量（基础类型）
     */
    bbt::cxxlua::Value GetGlobalValue(const char* value_name)
    {
        return GetValueEx(1, value_name);
    }

    /**
     * @brief 多级key索引的变量
     * 
     * @tparam TRetValue 
     * @tparam Args 
     * @param nkey 
     * @param args 
     * @return bbt::cxxlua::Value 
     */
    template<typename ... Args>
    bbt::cxxlua::Value GetValueEx(int nkey, Args ...args)
    {
        //TODO 需要bbt cxxlua 提供这个功能
        return bbt::cxxlua::Value(); // 默认返回nil
    }
protected:
    LuaScriptBaseComp(ecs::emComponentType type ,bbt::cxxlua::LuaVM* vm);

    /**
     * @brief 默认实现为调用Lua层ComponentMgr的Update事件
     */
    virtual void OnUpdate() override;
    void _SendLuaComponentMgrUpdateEvent();
private:
    void RegistInLuaEvent();
    void UnRegistInLuaEvent();

    std::string         m_script_path{""};      // 挂载的脚本路径
    bbt::cxxlua::LuaVM* m_vm{nullptr};          // 脚本运行的虚拟机
};

} // namespace share::ecs::luascript