#pragma once
#include <cluster/registery/RegisteryServer.hpp>
#include <cluster/registery/RegisteryBase.hpp>

namespace cluster
{

class RegisteryTcp : public RegisteryBase
{
public:
    explicit RegisteryTcp();
    virtual ~RegisteryTcp();

    void Init(const bbt::net::IPAddress& listen_addr, int timeout_ms);
    void Start();
    void Stop();

    virtual util::errcode::ErrOpt SendToNode(const char* uuid, const bbt::core::Buffer& buffer) override;
    virtual void OnError(const bbt::errcode::Errcode& err) = 0;
protected:
    virtual void OnNodeLoseConnection(bbt::network::ConnId connid);
private:
    std::shared_ptr<RegisteryServer> 
                        m_server{nullptr};
};

} // namespace cluster