#pragma once
#include <util/network/Connection.hpp>


namespace cluster
{
    
enum emRpcConnType
{
    RPC_CONN_TYPE_RN,   // registery 和 server 的连接
    RPC_CONN_TYPE_CR,   // client 和 registery 的连接
    RPC_CONN_TYPE_CS,   // client 和 server 的连接
};

}