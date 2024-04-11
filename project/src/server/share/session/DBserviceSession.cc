#include "util/log/Log.hpp"
#include "share/session/DBServiceSession.hpp"
#include "protocol/dbservice.pb.h"


namespace share::session
{


bool DBServiceSession::OnPing(bbt::buffer::Buffer req, bbt::buffer::Buffer& resp)
{
    DB_PING_REQ protobuf_req;
    DB_PING_RESP protobuf_resp;
    auto parse_succ = protobuf_req.ParseFromArray(req.Peek(), req.DataSize());
    if (!parse_succ) {
        GAME_EXT1_LOG_ERROR("parse DB_PING_REQ failed!");
        return false;
    }

    GAME_EXT1_LOG_DEBUG("[%d]", protobuf_req.timestamp());

    protobuf_resp.set_timestamp(bbt::timer::clock::now().time_since_epoch().count());
    auto string = protobuf_resp.SerializeAsString();
    resp.WriteString(string);

    return true;
}


}