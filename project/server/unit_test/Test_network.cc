#include "util/network/libevent/evIOCallbacks.hpp"

using namespace game::util::errcode;
using namespace game::util::network;
int main()
{
    ErrCode err;
    err.Set("nothing", Nothing, 1);
    OnAccept<ev::evConnection>(nullptr, err);
    OnConnect<ev::evConnection>(nullptr, err);

    // OnAccept<Connection>(nullptr, err);
    // OnConnect<Connection>(nullptr, err);
}