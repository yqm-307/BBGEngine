#pragma once
#include <cluster/registery/RegisterInfo.hpp>
#include <cluster/protocol/N2RProtocol.hpp>
#include <cluster/protocol/R2NProtocol.hpp>

namespace cluster
{

class RegisteryServer;

class Registery:
    public std::enable_shared_from_this<Registery>
{
public:
    explicit Registery();
    virtual ~Registery();

    void Init(const bbt::net::IPAddress& listen_addr, int timeout_ms);
    void Start();
    void Stop();

    util::errcode::ErrOpt           SendToNode(const char* uuid, const bbt::core::Buffer& buffer);
    util::errcode::ErrOpt           RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer);
    virtual void                    OnError(const bbt::errcode::Errcode& err) = 0;

    RegisterInfo*                   GetNodeRegInfo(std::string uuid);
    void                            CloseConn(bbt::network::ConnId connid);
private:
    virtual void                    OnNodeLoseConnection(bbt::network::ConnId connid);
    util::errcode::ErrOpt           N2RDispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt           OnHeartBeat(bbt::network::ConnId id, N2R_KeepAlive_Req* req);
    util::errcode::ErrOpt           OnHandshake(bbt::network::ConnId id, N2R_Handshake_Req* req);
private:
    std::shared_ptr<bbt::network::libevent::Network> m_network{nullptr};
    std::shared_ptr<RegisteryServer>            m_server{nullptr};

    std::unordered_map<std::string/*128 byte uuid*/, RegisterInfo>
                                                m_registery_map;
    std::unordered_set<bbt::network::ConnId>    m_helf_connect_set;
};

} // namespace cluster