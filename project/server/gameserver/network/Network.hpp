#include <event2/event.h>
#include <string>
#include "util/network/libevent/evIOThread.hpp"

namespace server::network
{

class Network
{
public:
    typedef game::util::network::ev::evIOThread IOThread;

    Network(const std::string& ip, short port);
    ~Network();
    bool SyncStart();
private:
    std::string     m_listen_ip;
    int             m_listen_port;
    IOThread*       m_accept_thread{nullptr};
    std::vector<IOThread*> m_io_threads;
};

}