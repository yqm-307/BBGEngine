#pragma once
#include <memory>

namespace game::server::util::alloc
{

/**
 * @brief alloc的全局hook,会hook掉 std operator new
 * 
 * @param nbytes 
 * @return void* 
 */
static void*   G_Hook_Alloc(size_t nbytes);

/**
 * @brief free的全局hook,会hook掉
 * 
 * @param ptr 
 */
static void    G_Hook_Free(void* ptr);

}