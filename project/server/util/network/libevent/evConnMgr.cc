#include "util/network/libevent/evConnMgr.hpp"

namespace game::util::network::ev
{

evConnMgr* evConnMgr::GetInstance()
{
    static evConnMgr* instance = nullptr;
    if(instance == nullptr)
        instance = new evConnMgr();
    
    return instance;
}

evConnMgr::evConnMgr()
{
}

evConnMgr::~evConnMgr()
{
}

ConnectionSPtr evConnMgr::CreateConn(int newfd, void* args)
{
    // std::make_shared<evConnection>();
    return nullptr;
}

ConnectionWKPtr evConnMgr::GetConnBySocket(int sockfd)
{
    return m_conn_map[sockfd];
}


}// namespace end