#include <event2/event.h>
#include <string>

namespace server::network
{

class Network
{
public:
    Network(const std::string& ip, short port);
    ~Network();
private:
    std::string     m_listen_ip;
    int             m_listen_port;
};

}