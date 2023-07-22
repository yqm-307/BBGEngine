#include "myalloc.hpp"
#include <malloc.h>

using namespace game::server::util::alloc;

void* G_Hook_Alloc(size_t nbytes)
{
    return malloc(nbytes);
}

void G_Hook_Free(void* ptr)
{
    free(ptr);
}