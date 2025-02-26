#include <bbt/base/clock/Clock.hpp>
#include <cluster/node/ClusterNode.hpp>

namespace cluster
{

RpcSerializer serializer;

ClusterNode::ClusterNode():
    m_rpc_clients(10),
    m_uuid(std::make_shared<util::other::Uuid>())
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
    if (auto err = InitNetwork(); err != std::nullopt)
        return err;

    m_registery_client = std::make_shared<RegisteryClient>(m_network, m_registery_addr);
    m_rpc_server = std::make_shared<RpcServer>(m_network, shared_from_this(), m_listen_addr, m_connect_timeout);
    
    auto connect_to_registery_err = m_registery_client->AsyncConnect([weak_this{weak_from_this()}](const bbt::errcode::Errcode& err, bbt::network::interface::INetConnectionSPtr conn){
        if (weak_this.expired())
            return;
        auto shared_this = weak_this.lock();
        if (err.IsErr()) {
            shared_this->OnError(err);
            return;
        }

        auto connid = conn->GetConnId();

        auto new_conn = std::make_shared<util::network::Connection>(std::static_pointer_cast<bbt::network::libevent::Connection>(conn), shared_this->m_connect_timeout);
        new_conn->SetCallbacks({
            .on_recv_callback = [weak_this, connid](bbt::network::libevent::ConnectionSPtr conn, const char* data, size_t len)
            {
                bbt::core::Buffer buffer(data, len);
                if (auto this_ptr = weak_this.lock(); this_ptr != nullptr) 
                    this_ptr->RecvFromRegistery(buffer);
            },
            .on_send_callback = [weak_this, connid](auto conn, const bbt::errcode::Errcode& err, size_t len)
            {
                if (auto this_ptr = weak_this.lock(); this_ptr != nullptr && err.IsErr())
                    this_ptr->OnError(err);
            },
            .on_close_callback = [weak_this, connid](auto conn, const bbt::net::IPAddress& addr)
            {
                if (auto this_ptr = weak_this.lock(); this_ptr != nullptr) {
                    this_ptr->OnCloseFromRegistery(connid, addr);
                }
            },
            .on_timeout_callback = [weak_this, connid](auto conn)
            {
                if (auto this_ptr = weak_this.lock(); this_ptr != nullptr)
                    this_ptr->OnTimeoutFromRegistey();
            },
            .on_err_callback = [weak_this, connid](void*, const bbt::errcode::Errcode& err)
            {
                if (auto this_ptr = weak_this.lock(); this_ptr != nullptr)
                    this_ptr->OnError(err);
            }
        });
        shared_this->m_registery_client->SetConn(new_conn);

        shared_this->N2R_DoHandshakeReq();
        
    });

    if (connect_to_registery_err != std::nullopt)
        return connect_to_registery_err;

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

void ClusterNode::RecvFromRegistery(bbt::core::Buffer& buffer)
{
    R2NProtocolHead* head = nullptr;

    // 协议校验
    {
        if (buffer.ReadableBytes() < sizeof(R2NProtocolHead)) {
            OnError(util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET));
            return;
        }
    
        head = (R2NProtocolHead*)buffer.Peek();
        if (buffer.ReadableBytes() < head->protocol_length) {
            OnError(util::errcode::ErrCode("buffer not enough", util::errcode::ErrType::RPC_IMCOMPLETE_PACKET));
            return;
        }
    }

    {
        if (!m_registery_connected && head->protocol_type != R2N_HANDSHAKE_RESP) {
            OnError(util::errcode::ErrCode("registery not handshake", util::errcode::ErrType::RPC_BAD_PROTOCOL));
            return;
        }
    }

    if (auto err = R2N_Dispatch(head->protocol_type, buffer.Peek(), head->protocol_length); err != std::nullopt) {
        OnError(err.value());
    }
}

void ClusterNode::OnTimeoutFromRegistey()
{
    OnInfo("[ClusterNode] registery lose connect! timeout!");
}

void ClusterNode::OnCloseFromRegistery(bbt::network::ConnId id, const bbt::net::IPAddress& addr)
{
    OnInfo("[ClusterNode] registery lose connect! addr=" + addr.GetIPPort());
    m_registery_connected = false;
    m_registery_client->DelConn();
    // TODO 稍后尝试重连
}

void ClusterNode::RecvFromNode(bbt::network::ConnId id, bbt::core::Buffer& buffer)
{
    OnInfo("recv info");
}

void ClusterNode::OnTimeoutFromNode(bbt::network::ConnId id)
{
    OnInfo("node timeout");
}

void ClusterNode::OnCloseFromNode(bbt::network::ConnId id)
{
    OnInfo("node close");
    m_rpc_client_connid_uuids.erase(id);
    m_rpc_clients.erase(m_rpc_client_connid_uuids[id]);
    m_rpc_server->DelConnect(id);
}

util::errcode::ErrOpt ClusterNode::InitNetwork()
{
    if (m_network != nullptr)
        return util::errcode::ErrCode("already init network!", util::errcode::ErrType::CommonErr);

    m_network = std::make_shared<bbt::network::libevent::Network>();
    auto err = m_network->AutoInitThread(2);
    if (err.IsErr())
        return util::errcode::ErrCode("init network failed! " + err.What(), util::errcode::ErrType::CommonErr);

    return std::nullopt;
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
        EasyCheck(type, sizeof(R2N_Handshake_Resp));
        return R2N_OnHandshakeResp((R2N_Handshake_Resp*)proto);
    default:
        return util::errcode::ErrCode("unknown protocol type=" + std::to_string(type), util::errcode::ErrType::RPC_UNKNOWN_PROTOCOL);
    }

#undef EasyCheck
}

util::errcode::ErrOpt ClusterNode::R2N_OnHandshakeResp(R2N_Handshake_Resp* resp)
{
    OnInfo("[CLusterNode] registery handshake success! msg_code=" + std::to_string(resp->msg_code));
    m_registery_connected = true;
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