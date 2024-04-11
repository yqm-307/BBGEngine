#pragma once
#include "share/ecs/network/DBServiceConnection.hpp"

namespace share::session
{

enum DBServiceProtoId
{
    EM_DB_SERVICE_PROTO_PING_REQ = 1, // ping
};

class DBServiceSession
{
public:
    static void Init()
    {
        ecs::network::DBServiceConnection::RegistHandler(EM_DB_SERVICE_PROTO_PING_REQ, (ecs::network::DBServiceCFuncPtr*)&DBServiceSession::OnPing);
    }

private:
    static bool OnPing(bbt::buffer::Buffer req, bbt::buffer::Buffer& resp);
};

}