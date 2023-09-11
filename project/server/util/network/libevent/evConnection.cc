#include "util/network/libevent/evConnection.hpp"
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/libevent/evIOThread.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"

namespace game::util::network::ev
{



evConnection::evConnection(IOThread* thread, int newfd, Address peer_ip, Address local_ip)
    :m_io_thread(thread),
    m_sockfd(newfd),
    m_peer_addr(peer_ip),
    m_local_addr(local_ip),
    m_prev_heart_beat_time(bbt::timer::clock::now()),
    m_recv_buffer(4096) // TODO 接入配置
{
    DebugAssert(thread != nullptr && newfd >= 0);
    OnInit();
}


evConnection::~evConnection()
{
    OnDestroy();
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
    return -1;
}

void evConnection::Close()
{
    // TODO 没有实现逻辑
    OnDestroy();
}

void evConnection::InitEvent()
{
    int err = 0;
    err = GetIOThread()->RegisterEvent(m_recv_event);

    DebugAssert(err >= 0);
}

void evConnection::InitEventArgs()
{
    /* 设置 Read 事件的监听函数 */
    m_recv_event = evEvent::Create([this](evutil_socket_t fd, short events, void* args){
        this->OnEvent(fd, events, args);
    }, m_sockfd, EV_READ | EV_PERSIST | EV_TIMEOUT | EV_CLOSED, 5000);  // TODO 配置socket空闲超时事件
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

void evConnection::OnInit()
{
    /* 初始化 event 事件 args */
    InitEventArgs();
    /* 初始化心跳、读事件 */
    InitEvent();
}

void evConnection::OnDestroy()
{   
    if(m_status == ConnStatus::Disconnected)
        return;

    m_status = ConnStatus::Disconnected;

    // 先执行 callback
    if(m_ondestory_cb)
        m_ondestory_cb(shared_from_this());

    int error = 0;

    error = GetIOThread()->UnRegisterEvent(m_recv_event->GetEventID());
    if(error < 0) {
        GAME_EXT1_LOG_ERROR("event register error! eventid=%d", m_recv_event->GetEventID());
    }
    DebugAssert(m_recv_event != nullptr);
    DebugAssert(error >= 0);

    ::close(m_sockfd);

    m_recv_event = nullptr;
}

std::pair<char*,size_t> evConnection::GetRecvBuffer()
{
    return {m_recv_buffer.Peek(), m_recv_buffer.WriteableBytes()};
}

evIOThread* evConnection::GetIOThread()
{
    return reinterpret_cast<evIOThread*>(m_io_thread);
}

evutil_socket_t evConnection::GetSocket()
{
    return m_sockfd;
}


void evConnection::SetOnDestory(const OnDestoryCallback& cb)
{
    DebugAssert(cb != nullptr);
    m_ondestory_cb = cb;
}

void evConnection::OnEvent(evutil_socket_t fd, short events, void* args)
{
    if(events & EV_READ) {
        EventHandler_OnRecv(fd, events, args);
    }
    else if (events & EV_TIMEOUT) {
        EventHandler_OnSocketTimeOut(fd, events, args);
    }
    else {
        EventHandler_OnClose(fd, events, args);
    }
}

void evConnection::EventHandler_OnClose(evutil_socket_t fd, short events, void* args)
{
    //TODO socket 断开事件
    Close();
}

void evConnection::EventHandler_OnSocketTimeOut(evutil_socket_t fd, short events, void* args)
{
    //TODO 连接超时事件
    GAME_BASE_LOG_WARN("[evConnection::EventHandler_OnSocketTimeOut] event handler is empty!");  // 没有实现
}

void evConnection::EventHandler_OnRecv(evutil_socket_t fd, short events, void* args)
{

    // DebugAssert(ev_cb->m_conn_ptr != nullptr);
    if(IsClosed()) {
        GAME_EXT1_LOG_WARN("conn is closed, but the event was not canceled! peer:{%s}", GetPeerIPAddress().GetIPPort().c_str());
        return;
    }

    int read_len = 0;
    auto [recv_buff, buff_len] = GetRecvBuffer();
    util::errcode::ErrCode errcode("nothing", 
        util::errcode::NetWorkErr, 
        util::errcode::network::err::Recv_Success);


    // read系统调用读取数据
    read_len = ::read(fd, recv_buff, buff_len);
    m_recv_buffer.WriteNull(read_len);
    // 读取后处理errno，将errno处理，并转换为errno
    if(read_len == -1) {
        int err = errno;
        if(err == EINTR || err == EAGAIN) {
            errcode.SetInfo("please try again!");
            errcode.SetCode(util::errcode::network::err::Recv_TryAgain);
        } else if (err == ECONNREFUSED) {
            errcode.SetInfo("connect refused!");
            errcode.SetCode(util::errcode::network::err::Recv_Connect_Refused);
        }
    }else if (read_len == 0) {
        errcode.SetInfo("connect refused!");
        errcode.SetCode(util::errcode::network::err::Recv_Eof);
    }else if (read_len < -1){
        errcode.SetInfo("please look up errno!");
        errcode.SetCode(util::errcode::network::err::Recv_Other_Err);
    }
    // 处理之后反馈给connection进行数据处理
    OnRecvEventDispatch(m_recv_buffer, errcode);
}

#pragma region "网络事件处理函数的实现"

void evConnection::NetHandler_RecvData(const bbt::buffer::Buffer& buffer)
{
    //FIXME 这里的读写并没有对当前连接的接收缓存进行清理操作
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


bbt::timer::clock::Timestamp<bbt::timer::clock::ms> 
evConnection::GetPrevHeartBeatTimestamp()
{
    return m_prev_heart_beat_time;
}

void evConnection::OnHeartBeat()
{
    m_prev_heart_beat_time = bbt::timer::clock::now();
}

#pragma endregion
}