#include <cluster/registery/RegisteryTcp.hpp>

namespace cluster
{

RegisteryTcp::RegisteryTcp()
{
};

RegisteryTcp::~RegisteryTcp()
{
};

void RegisteryTcp::Init(const bbt::net::IPAddress& listen_addr, int timeout_ms)
{
    m_server = std::make_shared<RegisteryServer>(this, listen_addr.GetIP(), listen_addr.GetPort(), timeout_ms);
    m_server->Init();
}

void RegisteryTcp::Start()
{
    m_server->Start();
}

void RegisteryTcp::Stop()
{
    m_server->Stop();
}

util::errcode::ErrOpt RegisteryTcp::SendToNode(const char* uuid, const bbt::core::Buffer& buffer)
{
    auto* node_info = GetNodeRegInfo(uuid);
    if (node_info == nullptr)
        return util::errcode::ErrCode("node not found! uuid=" + std::string{uuid}, util::errcode::ErrType::RPC_NOT_FOUND_NODE);

    auto conn = m_server->GetConnectById(node_info->GetConnId());
    if (conn == nullptr)
        return util::errcode::ErrCode("conn is losed!", util::errcode::ErrType::CommonErr);

    conn->Send(buffer.Peek(), buffer.ReadableBytes());
    return std::nullopt;
}

void RegisteryTcp::OnNodeLoseConnection(bbt::network::ConnId connid)
{
}

}