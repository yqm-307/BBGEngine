#pragma once
#include <cluster/registery/RegisterInfo.hpp>
#include <cluster/protocol/N2RProtocol.hpp>
#include <util/network/Connection.hpp>

namespace cluster
{

class RegisteryBase
{
public:
    RegisteryBase();
    virtual ~RegisteryBase() = default;

    virtual util::errcode::ErrOpt   RecvFromNode(bbt::core::Buffer& buffer) final;
    virtual util::errcode::ErrOpt   SendToNode(const char* uuid, const bbt::core::Buffer& buffer) = 0;
    virtual void OnError(const bbt::errcode::Errcode& err) = 0;

    virtual void OnNodeLoseConnection(bbt::network::ConnId connid) = 0;
private:
    util::errcode::ErrOpt           Dispatch(emN2RProtocolType type, void* proto, size_t proto_len);
    util::errcode::ErrOpt           OnHeartBeat(N2R_KeepAlive_Req* req);

private:
    std::unordered_map<std::string/*128 byte uuid*/, RegisterInfo> m_registery_map;
};

}