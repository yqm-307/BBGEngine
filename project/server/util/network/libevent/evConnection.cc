#include "util/network/libevent/evConnection.hpp"
#include "util/network/libevent/evIOCallbacks.hpp"
#include "util/network/libevent/evIOThread.hpp"
#include "util/log/Log.hpp"
#include "util/assert/Assert.hpp"

namespace util::network::ev
{



evConnection::evConnection(evIOThreadSPtr thread, int newfd, Address peer_ip, Address local_ip)
    :Connection(newfd, peer_ip, local_ip),
    m_io_thread(thread),
    m_prev_heart_beat_time(bbt::timer::clock::now()),
    m_recv_buffer(4096) // TODO 接入配置
{
    DebugAssert(thread != nullptr && newfd >= 0);
    /* 初始化 event 事件 args */
    InitEventArgs();
    /* 初始化心跳、读事件 */
    InitEvent();
    GAME_EXT1_LOG_DEBUG("evconnection is created!");
}


evConnection::~evConnection()
{
    OnDestroy();
    GAME_EXT1_LOG_DEBUG("evconnection is released!");
}

evConnectionSPtr evConnection::SPtr()
{
    return std::static_pointer_cast<evConnection>(shared_from_this());
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

void evConnection::SetOnRecv(const OnRecvCallback& onrecv_handler)
{
    DebugAssert(onrecv_handler != nullptr);
    m_onrecv = onrecv_handler;
}

void evConnection::SetOnSend(const OnSendCallback& onsend_handler)
{
    DebugAssert(onsend_handler != nullptr);
    m_onsend = onsend_handler;
}

void evConnection::SetOnClose(const OnCloseCallback& onclose_handler)
{
    DebugAssert(onclose_handler != nullptr);
    m_onclose = onclose_handler;
}


void evConnection::Close()
{
    if(m_onclose)
        m_onclose(util::errcode::ErrCode("", util::errcode::ErrType::NetWorkErr, util::errcode::network::Recv_Eof), SPtr());

    OnDestroy();
}

void evConnection::InitEvent()
{
    BBTATTR_COMM_Unused int err = 0;

    auto thread = GetIOThread();
    if(thread == nullptr) {
        GAME_EXT1_LOG_WARN("thread is already released!");
        return;
    }

    err = GetIOThread()->RegisterEvent(m_recv_event);

    DebugAssert(err >= 0);
}

void evConnection::InitEventArgs()
{

    auto weak_this = weak_from_this();
    /* 设置 Read 事件的监听函数 */
    m_recv_event = evEvent::Create([weak_this](evutil_socket_t fd, short events, void* args){
        auto share_this = std::static_pointer_cast<evConnection>(weak_this.lock());
        if(share_this == nullptr) {
            GAME_EXT1_LOG_WARN("connect is delete, but event is unregisted! fd=%d, fd=%d", fd, events);
            return;
        }
        share_this->OnEvent(fd, events, args);
    }, GetSocket(), EV_READ | EV_PERSIST | EV_CLOSED | EV_WRITE, 5000);
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

    size_t len = buffer.DataSize();
    /* 找对应的事件处理函数，去处理对应的网络事件 */
    it_handler->second(buffer);

    /* TODO  也许需要更多的灵活性，提供一个不清楚缓存的可能，目前认为recv buffer，在上面的handler是被处理完毕的 */
    /* 通知处理完了，清楚缓存 */
    if(m_recv_buffer.ReadableBytes() > 0)
    {
        m_recv_buffer.Recycle(m_recv_buffer.ReadableBytes());
    }

    /* 接收事件观察者存在，通知观察者 */
    if(m_onrecv)
        m_onrecv(err, len);
}

void evConnection::OnDestroy()
{   
    {
        std::lock_guard<std::mutex> lock(m_mutex);
        if(m_status == ConnStatus::Disconnected) {
            GAME_EXT1_LOG_WARN("repeat destory!");
            return;
        }

        m_status = ConnStatus::Disconnected;
    }

    int error = 0;
    
    auto thread = GetIOThread();
    if(thread == nullptr) {
        GAME_EXT1_LOG_WARN("thread is already released!");
        return;
    }

    error = thread->UnRegisterEvent(m_recv_event->GetEventID());
    if(error < 0) {
        GAME_EXT1_LOG_ERROR("event unregister error! eventid=%d", m_recv_event->GetEventID());
    }
    DebugAssert(error >= 0);

    ::close(GetSocket());

    m_recv_event = nullptr;
}

std::pair<char*,size_t> evConnection::GetRecvBuffer()
{
    return {m_recv_buffer.Peek(), m_recv_buffer.WriteableBytes()};
}

evIOThreadSPtr evConnection::GetIOThread()
{
    return m_io_thread.lock();
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
    Close();
}

void evConnection::EventHandler_OnSocketTimeOut(evutil_socket_t fd, short events, void* args)
{
    Close();
    GAME_EXT1_LOG_WARN("[evConnection::EventHandler_OnSocketTimeOut] Timeout ip{%s}", m_peer_addr.GetIPPort().c_str());

    if(m_timeout_cb)
        m_timeout_cb(SPtr());
}

void evConnection::EventHandler_OnRecv(evutil_socket_t fd, short events, void* args)
{

    // DebugAssert(ev_cb->m_conn_ptr != nullptr);
    if(IsClosed()) {
        GAME_EXT1_LOG_WARN("conn is closed, but the event was not canceled! peer:{%s}", GetPeerIPAddress().GetIPPort().c_str());
        return;
    }

    int read_len = 0;
    auto recv_buff = m_recv_buffer.Peek();
    auto buff_len = m_recv_buffer.WriteableBytes();
    util::errcode::ErrCode errcode("nothing", 
        util::errcode::NetWorkErr, 
        util::errcode::network::err::Recv_Success);


    /**
     * read系统调用读取数据，策略是根据本地缓存，尽量读
     */
    read_len = ::read(fd, recv_buff, buff_len);
    m_recv_buffer.WriteNull(read_len);
    /* 读取后处理errno，将errno处理，并转换为errno */
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
    /* 处理之后反馈给connection进行数据处理 */
    OnRecvEventDispatch(m_recv_buffer, errcode);
}

#pragma region "网络事件处理函数的实现"

void evConnection::NetHandler_RecvData(const bbt::buffer::Buffer& buffer)
{
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

size_t evConnection::Send(const char* buffer, size_t len)
{
    return -1;
}

size_t evConnection::Recv(const char* buffer, size_t len)
{
    return -1;
}


}