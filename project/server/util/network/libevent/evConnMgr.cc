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
    //FIXME 这里需要修改为正确的初始化，evConnection的构造和机制已经修改了。所以需要同步的处理
    auto new_conn = std::make_shared<evConnection>();
    //TODO 这里需要对新连接进行处理，比如加入evConnMgr的进行管理
    return nullptr;
}

ConnectionWKPtr evConnMgr::GetConnBySocket(int sockfd)
{
    return m_conn_map[sockfd];
}


}// namespace end