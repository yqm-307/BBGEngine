#include <bbt/base/clock/Clock.hpp>
#include <cluster/node/ClusterNode.hpp>

namespace cluster
{

RpcSerializer serializer;

ClusterNode::ClusterNode():
    m_rpc_clients(10)
{
}

ClusterNode::~ClusterNode()
{
}

void ClusterNode::Init(const bbt::net::IPAddress& listen_addr, const bbt::net::IPAddress& registery_addr, int timeout)
{
    m_listen_addr = listen_addr;
    m_registery_addr = registery_addr;
    m_connect_timeout = timeout;
    
}

util::other::Uuid::SPtr ClusterNode::GetUUID() const
{
    return m_uuid;
}

NodeState ClusterNode::GetNodeState()
{
    return m_state;
}

const std::string& ClusterNode::GetName() const
{
    return m_service_name;
}

util::errcode::ErrOpt ClusterNode::Start()
{
    m_network = std::make_shared<bbt::network::libevent::Network>();
    m_registery_client = std::make_shared<RegisteryClient>(m_network, m_registery_addr);
    m_rpc_server = std::make_shared<RpcServer>(m_network, shared_from_this(), m_listen_addr, m_connect_timeout);
    
    m_registery_client->AsyncConnect([=](const bbt::errcode::Errcode& err, bbt::network::interface::INetConnectionSPtr conn){
        if (err.IsErr()) {
            OnError(err);
            return;
        }

        auto connid = conn->GetConnId();

        auto new_conn = std::make_shared<util::network::Connection>(std::static_pointer_cast<bbt::network::libevent::Connection>(conn), m_connect_timeout);
        new_conn->SetCallbacks({
            .on_recv_callback = [weak_this{weak_from_this()}, connid](bbt::network::libevent::ConnectionSPtr conn, const char* data, size_t len)
            {
                auto this_ptr = weak_this.lock();
                if (this_ptr == nullptr)
                    return;

                bbt::core::Buffer buffer(data, len);
                this_ptr->RecvFromRegistery(buffer);
            },
            .on_send_callback = [weak_this{weak_from_this()}, connid](auto conn, const bbt::errcode::Errcode& err, size_t len)
            {
                auto this_ptr = weak_this.lock();
                if (this_ptr == nullptr)
                    return;

                if (err.IsErr()) {
                    this_ptr->OnError(err);
                    return;
                }
            },
            .on_close_callback = [weak_this{weak_from_this()}, connid](auto conn, const bbt::net::IPAddress& addr)
            {
                auto this_ptr = weak_this.lock();
                if (this_ptr == nullptr)
                    return;
                
                if (this_ptr->m_registery_client == nullptr)
                    return;
                
                this_ptr->m_registery_client->DelConn();
                this_ptr->OnCloseRegistery(connid, addr);
            },
            .on_timeout_callback = [weak_this{weak_from_this()}, connid](auto conn)
            {
                auto this_ptr = weak_this.lock();
                if (this_ptr == nullptr)
                    return;

                this_ptr->OnError(util::errcode::ErrCode{"[ClusterNode] registery connection timeout!", util::errcode::CommonErr});
            },
            .on_err_callback = [weak_this{weak_from_this()}, connid](void*, const bbt::errcode::Errcode& err)
            {
                auto this_ptr = weak_this.lock();
                if (this_ptr == nullptr)
                    return;

                this_ptr->OnError(err);
            }
        });
        m_registery_client->SetConn(new_conn);
    });

    m_rpc_server->Start();

    return std::nullopt;
}

std::shared_ptr<util::other::Uuid> ClusterNode::GetRandomUuidByMethod(const std::string& method)
{
    return std::make_shared<util::other::Uuid>();
}



void ClusterNode::Active()
{
    auto buffer = serializer.Serialize(
        N2R_KEEPALIVE_REQ,
        m_uuid->ToString(),
        bbt::clock::gettime() / 1000);

    // 发送心跳包
    SendToRegistery(buffer);
}

void ClusterNode::Offline()
{
    m_state = NODESTATE_OFFLINE;
}

void ClusterNode::Online()
{
    m_state = NODESTATE_ONLINE;
}

void ClusterNode::Clear()
{
    m_last_active_time = -1;
    m_service_name = "";
    m_state = NODESTATE_DEFAULT;
    m_uuid = nullptr;
}

util::errcode::ErrOpt ClusterNode::RemoteCall(const std::string& method, bbt::core::Buffer& buffer)
{
    // 找到本地方法

    // 解析协议头

    // 执行方法
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::OnRemoteCall(bbt::network::ConnId id, bbt::core::Buffer& reply)
{

    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::RecvFromRegistery(bbt::core::Buffer& buffer)
{
    R2NProtocolHead* head = nullptr;

    // 协议校验
    {
        if (buffer.ReadableBytes() < sizeof(R2NProtocolHead))
            return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);
    
        head = (R2NProtocolHead*)buffer.Peek();
        if (buffer.ReadableBytes() < head->protocol_length)
            return util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET);
    }

    {
        if (!m_registery_connected && head->protocol_type != R2N_HANDSHAKE_RESP)
            return util::errcode::ErrCode("registery not handshake", util::errcode::ErrType::RPC_BAD_PROTOCOL);
    }

    return R2N_Dispatch(head->protocol_type, buffer.Peek(), head->protocol_length);
}

util::errcode::ErrOpt ClusterNode::R2N_Dispatch(emR2NProtocolType type, void* proto, size_t proto_len)
{
#define EasyCheck(type, len) if (proto_len != len) return util::errcode::ErrCode("invalid protocol length type=" + std::to_string(type), util::errcode::ErrType::RPC_BAD_PROTOCOL);

    switch (type)
    {
    case R2N_KEEPALIVE_RESP:
        EasyCheck(type, sizeof(R2N_KEEPALIVE_RESP));
        return R2N_OnHeartBeatResp((R2N_KeepAlive_Resp*)proto);
    case R2N_HANDSHAKE_RESP:
        EasyCheck(type, sizeof(N2R_Handshake_Req));
        return R2N_OnHandshakeResp((R2N_Handshake_Resp*)proto);
    default:
        return util::errcode::ErrCode("unknown protocol type=" + std::to_string(type), util::errcode::ErrType::RPC_UNKNOWN_PROTOCOL);
    }

#undef EasyCheck
}

util::errcode::ErrOpt ClusterNode::R2N_OnHandshakeResp(R2N_Handshake_Resp* resp)
{
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::R2N_OnHeartBeatResp(R2N_KeepAlive_Resp* resp)
{
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::N2R_DoHandshakeReq()
{
    N2R_Handshake_Req req;
    bbt::core::Buffer buffer;

    req.head.protocol_type = N2R_HANDSHAKE_REQ;
    req.head.protocol_length = sizeof(N2R_Handshake_Req);
    req.head.timestamp_ms = bbt::clock::gettime();
    if (!m_uuid->ToByte(req.head.uuid, sizeof(req.head.uuid)))
        return util::errcode::ErrCode{"[ClusterNode] uuid to byte failed!", util::errcode::CommonErr};

    buffer.WriteString((char*)&req, sizeof(req));

    return SendToRegistery(buffer);
}

util::errcode::ErrOpt ClusterNode::N2R_DoHeatBeatReq()
{
    N2R_KeepAlive_Req req;
    bbt::core::Buffer buffer;

    req.head.protocol_type = N2R_KEEPALIVE_REQ;
    req.head.protocol_length = sizeof(N2R_KeepAlive_Req);
    req.head.timestamp_ms = bbt::clock::gettime();
    if (!m_uuid->ToByte(req.head.uuid, sizeof(req.head.uuid)))
        return util::errcode::ErrCode{"[ClusterNode] uuid to byte failed!", util::errcode::CommonErr};

    buffer.WriteString((char*)&req, sizeof(req));

    return SendToRegistery(buffer);
}

util::errcode::ErrOpt ClusterNode::SendToRegistery(bbt::core::Buffer& buffer)
{
    if (m_registery_client == nullptr)
        return util::errcode::ErrCode{"[ClusterNode] registery client is null!", util::errcode::CommonErr};

    auto conn = m_registery_client->GetConn();
    if (conn == nullptr)
        return util::errcode::ErrCode{"[ClusterNode] registery connection is null!", util::errcode::CommonErr};

    conn->Send(buffer.Peek(), buffer.DataSize());
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::SendToNode(bbt::network::ConnId id, bbt::core::Buffer& buffer)
{
    auto uuid = m_rpc_client_connid_uuids.find(id);
    if (uuid == m_rpc_client_connid_uuids.end())
        return util::errcode::ErrCode{"[ClusterNode] node uuid not found!", util::errcode::CommonErr};

    auto client = m_rpc_clients.find(uuid->second);
    if (client == nullptr)
        return util::errcode::ErrCode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    auto conn = client->second->GetConn();
    if (conn == nullptr)
        return util::errcode::ErrCode{"[ClusterNode] node connection is null!", util::errcode::CommonErr};

    conn->Send(buffer.Peek(), buffer.DataSize());
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::N2NDispatch(bbt::network::ConnId id, emN2NProtocolType type, void* proto, size_t proto_len)
{
    return std::nullopt;
}

util::errcode::ErrOpt ClusterNode::DoHeartBeat(bbt::network::ConnId id, N2N_KeepAlive_Req* req)
{
    return std::nullopt;
}

}