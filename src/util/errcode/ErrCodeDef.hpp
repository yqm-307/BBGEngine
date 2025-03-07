#pragma once
#include <cstring>
#include <bbt/core/errcode/Errcode.hpp>

namespace util::errcode
{

using namespace bbt::core::errcode;

enum emErr: ErrType
{
    Nothing     = 1,
    CommonErr   = 2,
    NetWorkErr = 100,   // 网络err
    NET_ACCEPT_ERR, // 网络accept错误
    NET_UNKNOWN_CONN, // 未知的连接
    RPC_ERR_BEGIN = 200, // rpc err
    RPC_IMCOMPLETE_PACKET, // rpc 不完整的数据包 
    RPC_UNKNOWN_PROTOCOL, // rpc 未知的协议
    RPC_BAD_PROTOCOL, // rpc 错误的协议
    RPC_NOT_FOUND_NODE, // rpc 未找到节点
    RPC_ERR_END, // rpc err
};

}// namespace end