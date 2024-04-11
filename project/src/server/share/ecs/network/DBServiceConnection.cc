#include "share/ecs/network/DBServiceConnection.hpp"
#include "util/log/Log.hpp"


namespace share::ecs::network
{

void DBServiceConnection::RegistHandler(int protoid, const DBServiceHandler& handler)
{
    auto [_, succ] = m_proto_handler_map.insert(std::make_pair(protoid, handler));
    Assert(succ);
}

bool DBServiceConnection::Dispatch(int protoid, bbt::buffer::Buffer& buf, bbt::buffer::Buffer& resp)
{
    auto it = m_proto_handler_map.find(protoid);
    if (it == m_proto_handler_map.end()) {
        GAME_EXT1_LOG_ERROR("unknow protoid [%d]", protoid);
        return;
    }

    it->second(buf, resp);
}



DBServiceConnection::DBServiceConnection(ConnMgr* mgr, bbt::network::libevent::ConnectionSPtr raw_conn, int timeout_ms):
    Connection(mgr, raw_conn, timeout_ms)
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

int DBServiceConnection::_HasAProtocol(const char* data, size_t remain_size)
{
    // 当前这条协议的总长度
    int length = 0;
    memcpy((char*)length, data, sizeof(int));
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