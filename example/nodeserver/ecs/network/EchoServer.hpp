#pragma once
#include <plugin/ecs/network/ServerComp.hpp>
#include <nodeserver/ecs/network/EchoConnection.hpp>

namespace BBTENGINE_NODE_NAMESPACE::network
{

class EchoService:
    public plugin::ecs::network::Server
{
    ComponentDeriveClassDef;
protected:
    EchoService(const std::string& ip, short port, int connect_timeout):
        plugin::ecs::network::Server(ip, port, connect_timeout)
    {}

    virtual void OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr new_conn) override
    {
        if (err.IsErr()) {
            GAME_EXT1_LOG_ERROR("accept failed! %s", err.CWhat());
            return;
        }
        
        auto conn = std::make_shared<EchoConnection>(new_conn, 4000);
        if (!AddConnect(conn)) {
            GAME_EXT1_LOG_ERROR("add connect failed!");
            return;
        }
    
        conn->SetOnClose([this](auto conn_id){
            DelConnect(conn_id);
        });
    
        GAME_EXT1_LOG_INFO("new connection: {%s}", new_conn->GetPeerAddress().GetIPPort().c_str());
    }
};

}