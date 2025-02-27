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
    friend class RegisteryServer;
public:
    explicit Registery();
    virtual ~Registery();

    void                            Init(const bbt::net::IPAddress& listen_addr, int timeout_ms);
    void                            Update();
    void                            Start();
    void                            Stop();

    util::errcode::ErrOpt           SendToNode(const util::other::Uuid& uuid, const bbt::core::Buffer& buffer);
    void                            RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer);
    virtual void                    OnError(const bbt::errcode::Errcode& err) = 0;
    virtual void                    OnInfo(const std::string& info) = 0;
    virtual void                    OnDebug(const std::string& info) = 0;

    RegisterInfo*                   GetNodeRegInfo(const util::other::Uuid& uuid);
    void                            CloseConn(bbt::network::ConnId connid);
private:
    // 与节点的网络事件
    util::errcode::ErrOpt           N2RDispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt           OnHeartBeat(bbt::network::ConnId id, N2R_KeepAlive_Req* req);
    util::errcode::ErrOpt           OnHandshake(bbt::network::ConnId id, N2R_Handshake_Req* req);

    void                            OnAccept(bbt::network::ConnId connid);
    void                            OnClose(bbt::network::ConnId connid);
    void                            OnTimeout(bbt::network::ConnId connid);

    // Node握手相关
    bool                            IsHalfConn(bbt::network::ConnId connid) const;

private:
    std::shared_ptr<bbt::network::libevent::Network> m_network{nullptr};
    std::shared_ptr<RegisteryServer>            m_server{nullptr};

    std::unordered_map<bbt::network::ConnId, util::other::Uuid>
                                                m_connid_uuid_map;
    std::unordered_map<util::other::Uuid/*128 byte uuid*/, RegisterInfo, util::other::Uuid::Hash>
                                                m_registery_map;
    std::unordered_set<bbt::network::ConnId>    m_half_connect_set;
};

} // namespace cluster