// #include "util/network/libevent/evConnection.hpp"
// #include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/libevent/evIOThread.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"

namespace game::util::network::ev
{



evConnection::evConnection(IOThread* thread, int newfd, Address peer_ip, Address local_ip)
    :m_io_thread(thread),
    m_sockfd(newfd),
    m_peer_addr(peer_ip),
    m_local_addr(local_ip)
{
    DebugAssert(thread != nullptr && newfd >= 0);
}


evConnection::~evConnection()
{
    Close();
    Destroy();
}

bool evConnection::IsClosed()
{
    return (m_status == ConnStatus::Disconnected);
}

const Address& evConnection::GetPeerIPAddress() const
{
    return m_peer_addr;
}

const Address& evConnection::GetLocalIPAddress() const
{
    return m_local_addr;
}

size_t evConnection::Send(const char* buffer, size_t len)
{
    // TODO 没有实现逻辑
    return 0;
}

size_t evConnection::Recv(const char* buffer, size_t len)
{
    // TODO 没有实现逻辑
}

void evConnection::Close()
{
    // TODO 没有实现逻辑
}

const event_base* evConnection::GetEvBase() const
{
    return m_ev_base;
}

void evConnection::InitEvent()
{
    int err = 0;
    timeval heart;
    err = GetIOThread()->Register_OnRecv(m_sockfd, [this](const bbt::buffer::Buffer& buffer, const util::errcode::ErrCode& err){
        this->OnRecvEventDispatch(buffer, err);
    });
    DebugAssert(err >= 0);
    err = GetIOThread()->Register_HeartBeat(m_sockfd, [this](const util::errcode::ErrCode& err){
        this->TimeOutHandler(err);
    });
    DebugAssert(err >= 0);
}

void evConnection::OnRecvEventDispatch(const bbt::buffer::Buffer& buffer, const util::errcode::ErrCode& err)
{
    if(err.GetErrType() != util::errcode::ErrType::NetWorkErr)
    {
        GAME_EXT1_LOG_ERROR("recv fatal! ErrType:%d\tErrCode:%d", err.GetErrType(), err.GetErrCode());
        return;
    }

    auto it_handler = m_errcode_handler.find(err.GetErrCode());
    if(it_handler == m_errcode_handler.end())
    {
        GAME_EXT1_LOG_ERROR("don`t know errcode. ErrCode:%d", err.GetErrCode());
        return;
    }
    /* 找对应的事件处理函数，去处理对应的网络事件 */
    it_handler->second(buffer);
}

void evConnection::Init()
{
    [[maybe_unused]] int error = 0;
    DebugAssert(m_ev_base != nullptr);
    /* 初始化心跳、读事件 */
    InitEvent();
}

void evConnection::Destroy()
{   
    [[maybe_unused]] int error = 0;

    DebugAssert(m_ev_base != nullptr);
    DebugAssert(m_recv_event != nullptr);
    m_ev_base = nullptr;    // 生命期控制权不在这里

    error = event_del(m_recv_event);
    DebugAssert(error >= 0);

    error = event_del(m_timeout_event);
    DebugAssert(error >= 0);

    event_free(m_recv_event);
    m_recv_event = nullptr;

    event_free(m_timeout_event);
    m_timeout_event = nullptr;

}

std::pair<char*,size_t> evConnection::GetRecvBuffer()
{
    return {m_recv_buffer, sizeof(m_recv_buffer)};
}

evIOThread* evConnection::GetIOThread()
{
    return reinterpret_cast<evIOThread*>(m_io_thread);
}


#pragma region "网络事件处理函数的实现"

void evConnection::NetHandler_RecvData(const bbt::buffer::Buffer& buffer)
{
    //TODO 测试逻辑
    DebugAssert(buffer.DataSize() > 0);
    auto s_view = buffer.View();
    std::string s(s_view.begin(), s_view.end());
    GAME_BASE_LOG_INFO("[test recv] network RECV: [%s]", s.c_str());
}

void evConnection::NetHandler_ConnClosed(const bbt::buffer::Buffer& buffer)
{
    Close();
}

void evConnection::NetHandler_TryAgain(const bbt::buffer::Buffer& buffer)
{
    //TODO 逻辑需要实现
}

void evConnection::NetHandler_OtherErr(const bbt::buffer::Buffer& buffer)
{
    //TODO 逻辑需要实现
}


#pragma endregion

#pragma region "心跳时间发生"

void evConnection::TimeOutHandler(const util::errcode::ErrCode& err)
{
    //TODO 可能把timeout事件抛给上层处理更好(使用回调的方式)
    GAME_EXT1_LOG_DEBUG("Heart beat! ip{%s:%d}", GetPeerIP().c_str(), GetPeerPort());
}

#pragma endregion
}