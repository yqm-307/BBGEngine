#pragma once
#include <cassert>

/* todo：实现一个带有stacktrace的Assert */

#ifdef Debug
#define DebugAssertWithInfo(expr, info) \
    assert( (expr) && (info) )
#else
#define DebugAssertWithInfo(expr, info) {}
#endif

#ifdef Debug
#define DebugAssert(expr) \
    assert( expr )
#else
#define DebugAssert(expr) {}
#endif

#define Assert(expr) \
    assert( expr )

#define AssertWithInfo(expr, info) \
    assert( (expr) && (info) )

namespace util::assert
{

}