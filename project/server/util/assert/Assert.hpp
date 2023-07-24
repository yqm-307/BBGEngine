#pragma once
#include <cassert>

namespace game::util::assert
{

#ifndef Release
#define AssertWithInfo(expr, info) \
    assert( (expr) && (info) )
#else
#define AssertWithInfo(expr, info) {}
#endif


#define Assert(expr) \
    assert( expr )

}