#pragma once
#include <cluster/registery/RegisterInfo.hpp>
#include <cluster/protocol/N2RProtocol.hpp>
#include <util/network/Connection.hpp>

namespace cluster
{

class RegisteryBase:
    std::enable_shared_from_this<RegisteryBase>
{
public:
    RegisteryBase();
    virtual ~RegisteryBase() = default;

    virtual util::errcode::ErrOpt   RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer) final;
    virtual util::errcode::ErrOpt   SendToNode(const char* uuid, const bbt::core::Buffer& buffer) = 0;
    virtual void OnError(const bbt::errcode::Errcode& err) = 0;

    virtual void                    OnNodeLoseConnection(bbt::network::ConnId connid) = 0;
    RegisterInfo*                   GetNodeRegInfo(std::string uuid);
    void                            CloseConn(bbt::network::ConnId connid);
private:
    // void                            OnConnClose(bbt::network::ConnId connid);
    util::errcode::ErrOpt           Dispatch(bbt::network::ConnId id, emN2RProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt           OnHeartBeat(bbt::network::ConnId id, N2R_KeepAlive_Req* req);
    util::errcode::ErrOpt           OnHandshake(bbt::network::ConnId id, N2R_Handshake_Req* req);

private:
    std::unordered_map<std::string/*128 byte uuid*/, RegisterInfo>  m_registery_map;
    std::unordered_set<bbt::network::ConnId>   m_helf_connect_set;
};

}