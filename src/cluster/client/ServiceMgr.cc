#include <cluster/client/ServiceMgr.hpp>

namespace cluster
{

void ServiceMgr::DelService(bbt::network::ConnId connid)
{
    auto it_uuid = m_rpc_client_connid_uuids.find(connid);
    if (it_uuid != m_rpc_client_connid_uuids.end())
    {
        auto it_service = m_service_map.find(it_uuid->second);
        if (it_service != m_service_map.end())
        {
            m_service_map.erase(it_service);
        }
        m_rpc_client_connid_uuids.erase(it_uuid);
    }
}

bool ServiceMgr::AddService(bbt::network::ConnId connid, const Service& service)
{
    auto it_uuid = m_rpc_client_connid_uuids.find(connid);
    if (it_uuid != m_rpc_client_connid_uuids.end())
    {
        return false;
    }

    m_rpc_client_connid_uuids[connid] = service.GetServiceUuid();
    m_service_map[service.GetServiceUuid()] = service;

    return true;
}

std::optional<util::other::Uuid> ServiceMgr::GetServiceUuidRandom(const std::string& method)
{
    // 遍历所有服务，查找包含该方法的服务。现在不考虑饿死
    auto it = m_service_map.begin();
    while (it != m_service_map.end())
    {
        if (it->second.GetMethods().find(method) != it->second.GetMethods().end())
        {
            return it->first;
        }
        ++it;
    }

    return util::other::Uuid();
}

bbt::network::ConnId ServiceMgr::GetServiceConnIdRandom(const std::string& method)
{
    // 遍历所有服务，查找包含该方法的服务。现在不考虑饿死
    auto it = m_service_map.begin();
    while (it != m_service_map.end())
    {
        if (it->second.GetMethods().find(method) != it->second.GetMethods().end())
        {
            for (auto& connid : m_rpc_client_connid_uuids)
            {
                if (connid.second == it->first)
                {
                    return connid.first;
                }
            }
        }
        ++it;
    }

    return -1;
}


}