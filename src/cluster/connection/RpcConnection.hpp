#pragma once
#include <memory>
#include <mutex>
#include <cluster/connection/ConnectionDef.hpp>
#include <util/network/Connection.hpp>

namespace cluster
{

template<class TListener>
class RpcConnection:
    public util::network::Connection
{
public:
    RpcConnection(emRpcConnType type, std::weak_ptr<TListener> listener, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms);
    virtual ~RpcConnection();

    emRpcConnType GetType() const;
protected:
    virtual void        OnRecv(const char* data, size_t len) override;
    virtual void        OnSend(util::errcode::ErrOpt err, size_t len) override;
    virtual void        OnClose() override;
    virtual void        OnTimeout() override;
    virtual void        OnError(const util::errcode::Errcode& err) override;
private:
    void                ProcessRecvBuffer();
    void                _SubmitRequest2Registery(std::vector<bbt::core::Buffer>& buffer);
private:
    std::weak_ptr<TListener> m_listener_wptr;
    bbt::core::Buffer       m_recv_buffer;
    const emRpcConnType     m_type;
    std::mutex              m_mutex;
};

} // namespace cluster

#include <cluster/connection/__TRpcConnection.hpp>