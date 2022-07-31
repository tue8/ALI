#include "mem.h"
#include <TlHelp32.h>
#include <iostream>

namespace mem 
{
	int get_process_id(const wchar_t* target_process, DWORD* process_id_buffer)
	{
		HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		PROCESSENTRY32 pe;

		if (!snap || snap == INVALID_HANDLE_VALUE) return 1;

		pe.dwSize = sizeof(pe);

		if (!Process32First(snap, &pe)) return 1;

		do
		{
			if (wcscmp(pe.szExeFile, target_process) == 0)
			{
				CloseHandle(snap);
				*process_id_buffer = pe.th32ProcessID;
				return 0;
			}
		} while (Process32Next(snap, &pe));

		return 1;
	}

	DWORD get_module_base_address(const wchar_t* module_name, DWORD process_id)
	{
		MODULEENTRY32 module_entry = { 0 };
		HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, process_id);

		if (!snapshot) return 1;

		module_entry.dwSize = sizeof(module_entry);

		if (!Module32First(snapshot, &module_entry)) return 1;

		do
		{
			if (!wcscmp(module_entry.szModule, module_name))
			{
				CloseHandle(snapshot);
				return (DWORD)module_entry.modBaseAddr;
			}
		} while (Module32Next(snapshot, &module_entry));

		CloseHandle(snapshot);
		return 1;
	}
}