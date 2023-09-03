#pragma once
#include <functional>
#include <event2/event.h>
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"

namespace game::util::network::ev
{

typedef std::function<void(evutil_socket_t, short, void*)>  EventCallback;

} // namespace end