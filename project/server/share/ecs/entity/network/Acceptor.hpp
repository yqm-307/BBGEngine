#pragma once
#include "util/network/IOThread.hpp"
#include "util/network/IPAddress.hpp"
#include "util/network/libevent/evEvent.hpp"
#include <functional>

namespace share::ecs::entity::network::__detail
{
static const int ListenCallback_Min_MS = 1000; // 如果没有事件，多久触发一次


/**
 * @brief 接收器
 *  监听指定ip、端口，并向上提供回调接口
 */
class Acceptor
{
public:
    typedef std::function<util::network::ev::evIOThreadSPtr()> LoadBlanceFunc;
    typedef std::function<void(util::network::ConnectionSPtr)> OnAcceptCallback;

    /**
     * @brief 构造一个接收器
     * 
     * @param ip 监听ip地址，如果指定为空字符串，监听ip地址则为 INADDR_ANY
     * @param port 监听端口
     */
    Acceptor(std::string ip, short port);
    ~Acceptor();

    /**
     * @brief 进行一次accept并处理新建立的连接
     * 
     * @return int 成功返回连接的fd；失败返回-1
     */
    int DoAccept();
    
    /**
     * @brief 暂停监听线程
     * 
     * @return int 成功返回0；失败返回-1
     */
    int Stop();

    /**
     * @brief 启动监听器，开始接收新连接。注意需要先注册OnAccept回调，否则连接会被自动释放
     * 
     * @return int 成功返回0；失败返回-1
     */
    int Start(util::network::ev::evIOThreadSPtr thread);

    /**
     * @brief 获取监听套接字
     */
    evutil_socket_t Fd() const;

    /**
     * @brief 获取监听地址
     */
    const std::string& IP() const;

    /**
     * @brief 获取监听端口
     */
    short Port() const;

    /**
     * @brief 设置连接到达时负载均衡回调
     * 
     * @param cb 
     */
    void SetLoadBlance(const LoadBlanceFunc& cb);

    /**
     * @brief 设置新连接建立时的回调
     * 
     * @param cb 
     */
    void SetOnConnect(const OnAcceptCallback& cb);
private:
    void Init();
    void Destory();
    void Clear();
    /* 接收到新连接，处理新连接 */
    void OnAccept(int fd, const util::network::Address& peer_addr);

    /**
     * @brief accept事件，定时触发获取新建立的连接
     * 
     * @param fd 事件相关文件描述符
     * @param event 事件类型
     * @param args 参数
     */
    void AcceptEvent(ev_socklen_t fd, short event, void* args);

private:
    evutil_socket_t m_listen_fd{-1};
    std::string     m_listen_ip{""};
    short           m_listen_port{-1};
    util::network::ev::evEventSPtr m_event{nullptr};
    util::network::Address    m_listen_addr;
    LoadBlanceFunc      m_load_blance_cb{nullptr};
    OnAcceptCallback    m_onconnect_cb{nullptr};
    int                 m_accept_once_timeval{50};  //TODO 配置文件
    util::network::ev::evIOThreadWKPtr m_master_thread;  // 所属的io线程
};

}// namespace end