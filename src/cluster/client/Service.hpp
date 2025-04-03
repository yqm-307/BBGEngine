#pragma once
#include <cluster/ClusterDefine.hpp>

namespace cluster
{

class Service
{
public:
    Service() = default;
    ~Service() = default;

    void UpsertMethod(const std::string& method);
    void DeleteMethod(const std::string& method);
    bbt::core::net::IPAddress GetServiceAddr() const;
    util::other::Uuid GetServiceUuid() const;
    std::string GetServiceName() const;
    std::unordered_set<std::string> GetMethods() const;

    void SetServiceAddr(const bbt::core::net::IPAddress& addr);
    void SetServiceUuid(const util::other::Uuid& uuid);
    void SetServiceName(const std::string& name);

private:
    std::string         m_service_name; // 服务名称
    util::other::Uuid   m_service_uuid; // 服务uuid
    bbt::core::net::IPAddress m_service_addr; // 服务地址

    std::unordered_set<std::string> m_methods_set; // 服务方法集合
};

} // namespace cluster