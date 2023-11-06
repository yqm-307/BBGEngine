#include "thirdparty/include/lua/lua.hpp"


int main()
{
    printf("begin\n");

    lua_State* ls = luaL_newstate(); 

    luaL_openlibs(ls);

    int err = luaL_loadstring(ls, "\
    function G_Print(szMsg) \
        print(szMsg)\
    end\
    ");



    if(err || lua_pcall(ls, 0, 0, 0)) {
        printf("[lua][error] %s\n", lua_tostring(ls, -1));
        return -1; 
    }



    if(lua_getglobal(ls, "G_Print") != LUA_TFUNCTION) {
        printf("[lua][error] %s\n", lua_tostring(ls, -1));
        return -1;
    }

    lua_pushstring(ls, "hello world");
    if(lua_pcall(ls, 1, 0, 0) != 0)
    {
        printf("[lua][error] %s\n", lua_tostring(ls, -1));
        return -1;
    }

    lua_close(ls);
    printf("end\n");
}