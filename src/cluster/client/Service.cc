#include <cluster/client/Service.hpp>


namespace cluster
{

void Service::UpsertMethod(const std::string& method)
{
    m_methods_set.insert(method);
}

void Service::DeleteMethod(const std::string& method)
{
    m_methods_set.erase(method);
}

bbt::core::net::IPAddress Service::GetServiceAddr() const
{
    return m_service_addr;
}

util::other::Uuid Service::GetServiceUuid() const
{
    return m_service_uuid;
}

std::string Service::GetServiceName() const
{
    return m_service_name;
}

std::unordered_set<std::string> Service::GetMethods() const
{
    return m_methods_set;
}

void Service::SetServiceAddr(const bbt::core::net::IPAddress& addr)
{
    m_service_addr = addr;
}

void Service::SetServiceUuid(const util::other::Uuid& uuid)
{
    m_service_uuid = uuid;
}

void Service::SetServiceName(const std::string& name)
{
    m_service_name = name;
}

} // namespace cluster