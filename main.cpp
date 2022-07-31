#include <iostream>
#include "luaE.h"
#include "mem.h"

static wchar_t* get_wc(const char* c)
{
	const size_t c_size = strlen(c) + 1;
	size_t out_size;
	wchar_t* wc = new wchar_t[c_size];
	mbstowcs_s(&out_size, wc, c_size, c, c_size - 1);
	return wc;
}

static HANDLE process_handler;

int luaE_memory_read(lua_State* L)
{
	int addr = lua_tonumber(L, 1);
	int x;

	if (!ReadProcessMemory(process_handler, (LPBYTE*)addr, &x,
		sizeof(x), NULL))
		return 0;

	lua_pushnumber(L, x);
	return 1; /* number of results */
}

int luaE_memory_write(lua_State* L)
{
	int addr = lua_tonumber(L, 1);
	int x = lua_tonumber(L, 2);

	if (!WriteProcessMemory(process_handler, (LPBYTE*)addr, &x,
		sizeof(x), NULL))
		return 0;

	lua_pushnumber(L, x);
	return 1; /* number of results */
}

static const luaL_reg memory_lib[] =
{
	{ "read", luaE_memory_read},
	{ "write", luaE_memory_write},
	{ NULL, NULL }
};


int main(int argc, char **argv)
{
	if (argc != 3)
	{
		std::cout << "Usage: ALI <process name> <script file name>" << std::endl;
		return 1;
	}

	DWORD process_id;
	wchar_t* process_name = get_wc(argv[1]);
	int result = mem::get_process_id(process_name, &process_id);

	delete process_name;

	if (result != 0)
	{
		std::cout << "Could not find process " << argv[1] << "." << std::endl;
		return 1;
	}

	luaE lua_embed;

	if (lua_embed.init() != 0)
	{
		std::cout << "Could not initialize lua." << std::endl;
		return 1;
	}

	lua_embed.add_lib("memory", memory_lib);

	process_handler = OpenProcess(PROCESS_ALL_ACCESS, NULL, process_id);

	std::cout << "Output:" << std::endl;

	if (lua_embed.load_script(argv[2]) != 0)
	{
		std::cout << "Could not load " << argv[2] << " script." << std::endl;
		return 1;
	}

	std::cout << "Successfully ran " << argv[2] << std::endl;

	return 0;
}