#include "DbgRpcServer.hpp"
#include "DbgRpcClient.hpp"


void DbgRpcServer::Send(const char* data, size_t size)
{
    auto parent = GetParentObject();
    if (parent)
    {
        auto client = parent->GetComponent<DbgRpcClient>();
        if (client)
        {
            client->OnReply(data, size);
        }
    }
}
