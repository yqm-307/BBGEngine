#pragma once
#include <engine/ecs/component/Component.hpp>
#include <bbt/network/adapter/libevent/Network.hpp>

namespace plugin::ecs::network
{

class IServerComp:
    public engine::ecs::Component
{
    ComponentDeriveClassDef;
public:
    IServerComp(engine::ecs::ComponentId id):
        engine::ecs::Component(id)
    {}

    /**
     * @brief 向 conn 发送数据
     * 
     * @param conn 
     * @param bytes 
     * @param len 
     */
    virtual size_t Send(bbt::network::ConnId conn, const char* bytes, size_t len) = 0;

    /**
     * @brief 尝试接收 conn 对端数据
     * 
     * @param conn 
     * @param recv_buf 
     * @param len 
     */
    virtual size_t Recv(bbt::network::ConnId conn, const char* recv_buf, size_t len) = 0;

    /**
     * @brief 关闭连接
     */
    virtual void ShowDown(bbt::network::ConnId conn) = 0;
};
    
}