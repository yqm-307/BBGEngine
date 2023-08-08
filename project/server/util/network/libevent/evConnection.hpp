#pragma once
#include "util/network/Connection.hpp"
#include "util/typedef/NamespaceType.hpp"

/**
 * @brief libevent实现
 * 
 */
namespace game::util::network::ev
{

class evConnection;
SmartPtrTypeDef(evConnection);

/**
 * @brief 可靠的双向连接，基于Tcp
 */
class evConnection:
    public Connection
{
public:
    evConnection();
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
    /* IO event 的注册和初始化 */
    void InitEvent();
    void OnRecv(int sockfd, short events, void* args);
    
private:
    event_base* m_ev_base;
    int         m_sockfd;
    ConnStatus  m_status;
    event*      m_ev_event;
    std::string m_ip;
    int         m_port;
};

}