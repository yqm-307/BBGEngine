#pragma once
#include <cluster/ClusterDefine.hpp>


namespace cluster
{

/**
 * @brief rpc 客户端服务信息管理类
 * 
 * 负责管理服务的信息，包括服务的uuid，连接id等
 * 
 */
class ServiceMgr
{
public:
    void GetMethods();
    void GetServiceInfoByMethod();
    void GetServiceInfoByUuid();
    void GetServiceInfoByConnId();
};

} // namespace cluster