#include "luaE.h"
#include <iostream>

int luaE::init()
{
    L = lua_open();
    if (L == NULL) return 1;
    return 0;
}

void luaE::add_lib(const char* lib_name, const luaL_Reg* lib_regs)
{
    luaL_openlibs(L);
    luaL_register(L, lib_name, lib_regs);
}

int luaE::load_script(const char* script_name)
{
    if (luaL_loadfile(L, script_name) == LUA_ERRFILE || lua_pcall(L, 0, LUA_MULTRET, 0) != 0) return 1;
    return 0;
}

luaE::~luaE()
{
    lua_close(L);
}
