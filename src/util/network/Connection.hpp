#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>
#include <util/errcode/ErrCode.hpp>

namespace util::network
{

class Server;

class Connection
{
    friend class TcpServer;
public:
    virtual ~Connection();
    Connection(bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms);
    bbt::network::ConnId GetConnId();
    
    // virtual void    Init() = 0;
    virtual void    SetCallbacks(bbt::network::libevent::ConnCallbacks callbacks);
    virtual void    Send(const char* data, size_t len);
    virtual bool    IsClosed();
    virtual void    Close();
private:
    void            SetOnClose(std::function<void(bbt::network::ConnId)> onclose);
    virtual void    _OnClose();
private:
    static uint64_t GenerateId() { return m_id_template++; }

    bbt::network::libevent::ConnCallbacks   m_conn_callbacks;
    bbt::network::ConnId                    m_conn_id{0};
    bbt::network::libevent::ConnectionSPtr  m_raw_conn_ptr{nullptr};
    std::function<void(bbt::network::ConnId)> m_onclose_cb{nullptr};

    static uint64_t                         m_id_template;
};

}