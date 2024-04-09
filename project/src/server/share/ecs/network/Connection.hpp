#pragma once
#include <bbt/network/adapter/libevent/Network.hpp>

namespace share::ecs::network
{
/**
 * @brief 可以继承自bbt::network::libevent::Connection
 * 但是选择使用组合的方式，因为继承层次可能有些多了，不是很好看
 */
class Connection
{
public:
    virtual ~Connection();
    Connection(bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms);
protected:
    void            OnRecv(const char* data, size_t len);
    void            OnSend(const bbt::network::Errcode& err, size_t succ_len);
    void            OnTimeout();
    void            OnClose();
    void            OnError(const bbt::network::Errcode& err);

private:
    bbt::network::libevent::ConnCallbacks   m_conn_callbacks;
    bbt::network::ConnId                    m_conn_id{0};
    bbt::network::libevent::ConnectionSPtr  m_raw_conn_ptr{nullptr};
};

}