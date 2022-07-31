#pragma once

extern "C"
{
	#include <lua.h>
	#include <lualib.h>
	#include <lauxlib.h>
}

class luaE
{
private:
	lua_State* L;
	bool has_script;
public:
	luaE() : L(NULL), has_script(true) {}
	int init();
	void add_lib(const char* lib_name, const luaL_Reg* lib_regs);
	int load_script(const char *script_name);
	~luaE();
};