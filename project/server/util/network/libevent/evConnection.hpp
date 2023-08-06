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

protected:

private:
    int         m_sockfd;
    ConnStatus  m_status;
    event*      m_event;
    std::string m_ip;
    int         m_port;
};

}