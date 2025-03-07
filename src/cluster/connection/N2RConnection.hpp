#pragma once
#include <cluster/connection/ConnectionDef.hpp>

namespace cluster
{

/**
 * node to registery connection
 */
class N2RConnection:
    public util::network::Connection
{
public:
    N2RConnection(std::weak_ptr<ClusterNode> node, bbt::network::libevent::ConnectionSPtr conn, int timeout_ms);
    virtual ~N2RConnection();

protected:

    virtual void        OnRecv(const char* data, size_t len) override;
    virtual void        OnSend(util::errcode::ErrOpt err, size_t len) override;
    virtual void        OnClose() override;
    virtual void        OnTimeout() override;
    virtual void        OnError(const util::errcode::Errcode& err) override;
private:
    void                ProcessRecvBuffer();
    void                _SubmitRequest2Node(std::vector<bbt::core::Buffer>& buffer);
private:
    std::weak_ptr<ClusterNode> m_node;
    std::mutex m_mutex;
    bbt::core::Buffer m_recv_buffer;
};

}