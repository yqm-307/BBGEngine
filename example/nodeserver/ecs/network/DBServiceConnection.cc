#include "plugin/scene/SceneDefine.hpp"
#include "plugin/ecs/network/NetworkComponent.hpp"
#include "nodeserver/ecs/network/DBServiceConnection.hpp"
#include "util/log/Log.hpp"


namespace share::ecs::network
{

std::unordered_map<int, DBServiceCPPFuncPtr> DBServiceConnection::m_proto_handler_map = std::unordered_map<int, DBServiceCPPFuncPtr>();

void DBServiceConnection::RegistHandler(int protoid, DBServiceCPPFuncPtr handler)
{
    auto [_, succ] = m_proto_handler_map.insert(std::make_pair(protoid, handler));
    Assert(succ);
}

bool DBServiceConnection::Dispatch(int protoid, bbt::buffer::Buffer& buf, bbt::buffer::Buffer& resp)
{
    auto it = m_proto_handler_map.find(protoid);
    if (it == m_proto_handler_map.end()) {
        GAME_EXT1_LOG_ERROR("unknow protoid [%d]", protoid);
        return false;
    }

    auto func = it->second;
    return func(buf, resp);
}



DBServiceConnection::DBServiceConnection(bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms):
    Connection(raw_conn, timeout_ms)
{
}

DBServiceConnection::~DBServiceConnection()
{
}

void DBServiceConnection::OnRecv(const char* data, size_t len)
{
    Assert(m_recv_buffer.WriteString(data, len));
    bbt::buffer::Buffer protocol;
    bbt::buffer::Buffer resp;
    auto [succ, protoid] = GetAProtocol(protocol);
    if (!succ)
        return;
    
    auto need_send = Dispatch(protoid, protocol, resp);

    if (need_send && resp.DataSize() > 0)
        Send(resp.Peek(), resp.DataSize());
}

void DBServiceConnection::OnClose()
{
    GAME_EXT1_LOG_WARN("lost db service connection!");
    auto network_obj = share::scene::GetGlobalInstByTid(EM_ENTITY_TYPE_NETWORK);
    AssertWithInfo(network_obj != nullptr, "");

    auto comp = network_obj->GetComponent(EM_COMPONENT_TYPE_NETWORK);
    auto network_comp = std::dynamic_pointer_cast<share::ecs::network::NetworkComponent>(comp);
    AssertWithInfo(comp != nullptr && network_comp != nullptr, "");
    network_comp->DelConnect(GetConnId());
}

void DBServiceConnection::OnTimeout()
{
    GAME_EXT1_LOG_WARN("db service timeout!");
    Close();
}



int DBServiceConnection::_HasAProtocol(const char* data, size_t remain_size)
{
    // 当前这条协议的总长度
    uint32_t length = 0;
    memcpy((char*)&length, data, sizeof(int));
    Assert(length >= 0);

    return (remain_size >= length ? length : -1);
}

std::pair<bool, int> DBServiceConnection::GetAProtocol(bbt::buffer::Buffer& protocol)
{
    int length = _HasAProtocol(m_recv_buffer.Peek(), m_recv_buffer.DataSize());
    if (length < 0)
        return {false, 0};
    
    protocol.WriteNull(length);
    m_recv_buffer.ReadString(protocol.Peek(), length);

    // 取出协议头
    protocol.ReadInt32();
    int protoid = protocol.ReadInt32();
    Assert(protoid >= 0);

    return {true, protoid};
}

}