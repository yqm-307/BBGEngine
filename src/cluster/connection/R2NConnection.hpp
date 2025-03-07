#pragma once
#include <cluster/connection/ConnectionDef.hpp>

namespace cluster
{

class Registery;

class R2NConnection:
    public util::network::Connection
{
public:
    R2NConnection(std::weak_ptr<Registery> registery, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms);
    virtual ~R2NConnection();

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
    std::weak_ptr<Registery> m_registery;
    std::mutex m_mutex;
    bbt::core::Buffer m_recv_buffer;
};

} // namespace cluster