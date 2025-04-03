#pragma once
#include <cluster/client/Service.hpp>


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

    std::optional<util::other::Uuid> GetServiceUuidRandom(const std::string& method);
    bbt::network::ConnId GetServiceConnIdRandom(const std::string& method);

    void DelService(bbt::network::ConnId connid);
    bool AddService(bbt::network::ConnId connid, const Service& service);
private:
    std::unordered_map<bbt::network::ConnId, util::other::Uuid> m_rpc_client_connid_uuids; // 连接id到uuid的映射
    std::unordered_map<util::other::Uuid, Service, util::other::Uuid::Hash> m_service_map; // uuid到服务的映射
};

} // namespace cluster