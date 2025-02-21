#pragma once
#include <util/network/Connection.hpp>

namespace cluster
{

class RegisteryBase;

/**
 * @brief NRConnection
 * 节点和注册中心的连接
 */
class NRConnection:
    public util::network::Connection
{
public:
    NRConnection(RegisteryBase* base, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms);

    virtual ~NRConnection() = default;
protected:
    virtual void    OnRecv(const char* data, size_t len);
    virtual void    OnSend(const bbt::errcode::Errcode& err, size_t succ_len);
    virtual void    OnTimeout();
    virtual void    OnClose();
    virtual void    OnError(const bbt::errcode::Errcode& err);
private:
    RegisteryBase*  m_registery{nullptr};   // 注册中心
};

} // namespace cluster