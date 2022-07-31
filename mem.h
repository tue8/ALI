#pragma once
#include <Windows.h>

namespace mem
{
	int get_process_id(const wchar_t* target_process, DWORD *process_id_buffer);
	DWORD get_module_base_address(const wchar_t* module_name, DWORD process_id);
}