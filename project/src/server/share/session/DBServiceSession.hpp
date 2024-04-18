#pragma once
#include "share/ecs/network/DBServiceConnection.hpp"

namespace share::session
{

#define REGIST_HANDLE(em, func) \
    ecs::network::DBServiceConnection::RegistHandler( \
        em,  \
        [=](bbt::buffer::Buffer req, bbt::buffer::Buffer& resp) \
        { \
            return func(req, resp); \
        } \
    );


enum DBServiceProtoId
{
    EM_DB_SERVICE_PROTO_PING_RESQ                   = 2, // ping resp
    EM_DB_SERVICE_PROTO_HEARTBEAT_RESP              = 4, // heart beat
};

class DBServiceSession
{
public:
    static void Init()
    {
        REGIST_HANDLE(EM_DB_SERVICE_PROTO_PING_RESQ,                OnPing);
        REGIST_HANDLE(EM_DB_SERVICE_PROTO_HEARTBEAT_RESP,           OnHeartBeart);
    }

private:
    static bool OnPing(bbt::buffer::Buffer req, bbt::buffer::Buffer& resp);
    static bool OnHeartBeart(bbt::buffer::Buffer req, bbt::buffer::Buffer& resp);
};

}