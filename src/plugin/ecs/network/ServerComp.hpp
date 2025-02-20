#pragma once
#include <plugin/ecs/network/IServerComp.hpp>
#include <plugin/ecs/network/Connection.hpp>

namespace plugin::ecs::network
{

/**
 * @brief 此组件提供网络通信功能 
 * 
 */
class Server:
    public IServerComp
{
public:
    ~Server();
    virtual size_t  Send(bbt::network::ConnId conn, const char* bytes, size_t len) override;
    BBTATTR_FUNC_DeprecatedMsg("recv message use event callback")
    virtual size_t  Recv(bbt::network::ConnId conn, const char* recv_buf, size_t len) override;
    virtual void    ShowDown(bbt::network::ConnId conn) override;

    virtual void Init() override;
    void SetListenAddr(const char* ip, short port);
    void Start();
    void Stop();
    bool Connect(const char* ip, short port, int timeout, const bbt::network::interface::OnConnectCallback& on_connect);

    bool DelConnect(bbt::network::ConnId conn);
    bool AddConnect(std::shared_ptr<Connection> conn);
    void OnTimeout(Connection* conn);
    std::shared_ptr<Connection> GetConnectById(bbt::network::ConnId conn_id);

protected:
    Server(const std::string& ip, short port, int connect_timeout);
    virtual void OnUpdate() override {}
    /**
     * @brief 重载此函数来处理新连接，默认新连接到来建立一个基础连接（只收发数据）
     */
    virtual void OnAccept(const bbt::network::Errcode& err, bbt::network::libevent::ConnectionSPtr conn) final;
    virtual std::shared_ptr<Connection> CreateConnection(bbt::network::libevent::ConnectionSPtr conn) = 0;
private:
    /* 连接管理 */
    bbt::network::libevent::Network*        m_network{nullptr};
    bbt::network::libevent::OnAcceptCallback m_onaccept_cb{nullptr};
    std::unordered_map<engine::ecs::ComponentId, std::shared_ptr<Connection>>
                                            m_conn_map;
    bbt::net::IPAddress                     m_listen_addr;
    int                                     m_connect_timeout{1000};
};

}