#include "DbgRpcClient.hpp"
#include "DbgRpcServer.hpp"

int DbgRpcClient::Send(const bbt::core::Buffer& buffer)
{
    auto father = GetParentObject();
    auto comp = father->GetComponent<DbgRpcServer>();
    if (comp == nullptr)
        return -1;

    auto send_buf = buffer;
    auto err = comp->OnRpc(send_buf);
    return 1;
}