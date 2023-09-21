#pragma once
#include <functional>
#include <event2/event.h>
#include <bbt/Attribute.hpp>
#include "util/assert/Assert.hpp"
#include "util/log/Log.hpp"

namespace util::network::ev
{

typedef std::function<void(evutil_socket_t, short, void*)>  EventCallback;

typedef int32_t EventId;

} // namespace end