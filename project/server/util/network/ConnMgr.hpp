#pragma once
#include "util/network/Connection.hpp"
#include "util/managerbase/ManagerBase.hpp"
#include "./Define.hpp"


#define evConnectionDeriveClassDef \
    FlagManagedByManagerBase(util::network::ConnectId, util::network::Connection)

namespace util::network
{

namespace ev
{
class evConnection;
}

class ConnMgr:
    public util::managerbase::ManagerBase<ConnectId, Connection>
{
public:
    ConnMgr();
    virtual ~ConnMgr() = 0;

    /* 创建Conn的接口 */
    // virtual ConnectionSPtr CreateConn(int newfd, void* args) = 0;

    virtual Result Search(KeyType key) override final;
    virtual bool IsExist(KeyType key) override final;
protected:
    virtual bool OnMemberCreate(MemberPtr member) override final;

    virtual bool OnMemberDestory(KeyType member) override final;

    virtual KeyType GenerateKey(MemberPtr) override final;

    /* 通过socketfd获取连接 */
    virtual ConnectionWKPtr GetConnBySocket(int sockfd) = 0;
private:
    std::map<ConnectId, ConnectionWKPtr> m_conn_map;
    std::mutex m_conn_map_mutex;
};

}// namespace end