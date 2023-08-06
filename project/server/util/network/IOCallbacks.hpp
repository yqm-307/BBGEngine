#pragma once
#include "util/network/Connection.hpp"
#include "util/errcode/ErrCode.hpp"
#include <type_traits>

namespace game::util::network
{

template<class ConnectionType>
void OnAccept(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    static_assert( std::is_base_of_v<Connection, ConnectionType> ); // 必须派生自Connection
    static_assert( ! std::is_same_v<ConnectionType, Connection> );  // 必须不可以为Connection
}

template<class ConnectionType>
void OnConnect(ConnectionSPtr new_conn, const errcode::ErrCode& err)
{
    static_assert( std::is_base_of_v<Connection, ConnectionType> ); // 必须派生自Connection
    static_assert( ! std::is_same_v<ConnectionType, Connection> );  // 必须不可以为Connection
}

}// namespace end