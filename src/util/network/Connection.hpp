#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>

namespace util::network
{

class Server;

class Connection:
    public std::enable_shared_from_this<Connection>
{
    friend class TcpServer;
public:
    virtual ~Connection();
    Connection(bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms);
    bbt::network::ConnId GetConnId();
    
    void                Init();
    virtual void        Send(const char* data, size_t len);
    virtual bool        IsClosed();
    virtual void        Close();
    const bbt::net::IPAddress& GetPeerAddr() const;

    virtual void        OnRecv(const char* data, size_t len) = 0;
    virtual void        OnSend(bbt::errcode::ErrOpt err, size_t len) = 0;
    virtual void        OnClose() = 0;
    virtual void        OnTimeout() = 0;
    virtual void        OnError(const bbt::errcode::Errcode& err) = 0;
private:
    void                SetOnCloseNotifyToTcpServer(std::function<void(bbt::network::ConnId)> onclose);
    void                _OnClose();

private:
    bbt::network::libevent::ConnectionSPtr  m_raw_conn_ptr{nullptr};
    std::function<void(bbt::network::ConnId)> m_notify_close_to_tcpserver_cb{nullptr};
};

}