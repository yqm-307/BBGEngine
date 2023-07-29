#pragma once
#include <cassert>

namespace game::util::assert
{

#ifdef Debug
#define AssertWithInfo(expr, info) \
    assert( (expr) && (info) )
#else
#define AssertWithInfo(expr, info) {}
#endif

#ifdef Debug
#define Assert(expr) \
    assert( expr )
#else
#define Assert(expr) {}
#endif

}