#pragma once
#include <map>
#include <unordered_map>
#include <util/network/Connection.hpp>

namespace util::network
{

typedef std::function<std::shared_ptr<Connection>(bbt::network::libevent::ConnectionSPtr)> ConnectionCreator;

class TcpServer:
    public std::enable_shared_from_this<TcpServer>
{
public:
    TcpServer(std::shared_ptr<bbt::network::libevent::Network> network, const std::string& ip, short port, int connect_timeout);
    virtual ~TcpServer();
    virtual size_t          Send(bbt::network::ConnId conn, const char* bytes, size_t len);
    BBTATTR_FUNC_DeprecatedMsg("recv message use event callback")
    virtual size_t          Recv(bbt::network::ConnId conn, const char* recv_buf, size_t len);
    virtual void            ShowDown(bbt::network::ConnId conn);
    virtual void            Init(ConnectionCreator creator);
    void                    Start();
    void                    Stop();
    bool                    DelConnect(bbt::network::ConnId conn);
    std::shared_ptr<Connection> GetConnectById(bbt::network::ConnId conn_id);
    const bbt::net::IPAddress&  GetListenAddr() const;
    std::optional<bbt::errcode::Errcode> SetListenAddr(const char* ip, short port);
    
protected:
    /**
     * @brief 重载此函数来处理新连接，默认新连接到来建立一个基础连接（只收发数据）
     */
    virtual void OnAccept(bbt::errcode::ErrOpt err, bbt::network::interface::INetConnectionSPtr conn) final;
    bool AddConnect(std::shared_ptr<Connection> conn);
private:
    /* 连接管理 */
    std::shared_ptr<bbt::network::libevent::Network> m_network{nullptr};
    bbt::network::libevent::OnAcceptCallback m_onaccept_cb{nullptr};
    std::unordered_map<bbt::network::ConnId, std::shared_ptr<Connection>>
                                            m_conn_map;
    bbt::net::IPAddress                     m_listen_addr;
    int                                     m_connect_timeout{1000};
    ConnectionCreator                       m_conn_creator{nullptr};
};

}