#include <memory>

namespace game::server::util::alloc
{

/**
 * @brief alloc的全局hook,会hook掉 std operator new
 * 
 * @param nbytes 
 * @return void* 
 */
void*   G_Hook_Alloc(size_t nbytes);

/**
 * @brief free的全局hook,会hook掉
 * 
 * @param ptr 
 */
void    G_Hook_Free(void* ptr);

}