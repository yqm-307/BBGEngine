#pragma once
#include "util/network/IOCallbacks.hpp"
#include "util/network/libevent/evConnection.hpp"

namespace game::util::network
{

template<>
void OnAccept<ev::evConnection>(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    printf("OnAccept 对于 evConnection 的专用化!\n");
}

template<>
void OnConnect<ev::evConnection>(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    printf("OnConnect 对于 evConnection 的专用化!\n");
}

}