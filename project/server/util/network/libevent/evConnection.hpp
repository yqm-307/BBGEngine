#pragma once
#include "util/network/Connection.hpp"
#include "util/typedef/NamespaceType.hpp"

/**
 * @brief libevent实现
 * 
 */
namespace game::util::network
{

static const int evConnection_Timeout_MS = 3000;

void OnRecvCallback(int sockfd, short events, void* args);
void OnHeartBeatCallback(evutil_socket_t sockfd, short events, void* args);

namespace ev
{


class evConnection;
SmartPtrTypeDef(evConnection);

/**
 * @brief 可靠的双向连接，基于Tcp
 */
class evConnection:
    public Connection
{
    friend void game::util::network::OnRecvCallback(int sockfd, short events, void* args);
    friend void game::util::network::OnHeartBeatCallback(evutil_socket_t sockfd, short events, void* args);
    friend class evConnMgr;
public:
    virtual ~evConnection();

    virtual bool IsClosed() override;
    virtual void Close() override;
    virtual size_t Send(const char* buffer, size_t len) override;
    virtual std::optional<std::string>  GetPeerIP() override;
    virtual std::optional<std::string>  GetLocalIP() override;
    virtual std::optional<int>          GetPeerPort() override;
    virtual std::optional<int>          GetLocalPort() override;
    /* 获取所在的 event_base */
    const event_base*                   GetEvBase() const;
protected:
    evConnection(event_base* ev_base, int newfd, std::string peer_ip, short port);

    void Init();
    void Destroy();
    /* IO event 的注册和初始化 */
    void InitEvent();
    void OnRecv(int sockfd);
    
private:
    event_base* m_ev_base;
    int         m_sockfd;
    ConnStatus  m_status;
    event*      m_recv_event;   // 接收事件
    event*      m_timeout_event;// 超时事件
    // event*      m_ev_send;
    std::string m_ip;
    short       m_port;
};
}

}