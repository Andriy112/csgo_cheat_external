#include <assert.h>
#include "offsets.h"
#include "windows.h"
#include <tlhelp32.h>
#include <stdbool.h>
#include "player.h"
#include "cheat_launcher.h"

int start(struct colors* pcolors)
{
	player* me = get_next_entity_obj(0);
	DWORD32 glowObjManager, glowIndex;
#pragma region trying to read glowObjectManager
	assert(ReadProcessMemory(setup.proc, (LPCVOID)(setup.Module + dwGlowObjectManager), &glowObjManager, 4, 0));
#pragma endregion
	player* __player;
	for (;;)
	{
		for (size_t i = 1; i <= 64; i++)
		{
			__player = get_next_entity_obj(i);
			//returns iterated player's glowIndex
			ReadProcessMemory(setup.proc, (LPCVOID)(((DWORD32)__player->address) + m_iGlowIndex), &glowIndex, 4, 0);
			if (__player->teamNum != me->teamNum)
			{
				put_wallhack(&glowObjManager, &glowIndex, pcolors);
				//if local player's crosshair on enemy , then you must shot
				if (me->on_enemy())
				{
					me->fire();
				}
				delete __player;
				continue;
			}
			else
			{
				delete __player;
				continue;
			}
		}

	}

}
void set_options(struct csgo_setup* psetup)
{
	psetup->procId = getprocId(L"csgo.exe");
	psetup->proc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, psetup->procId);

	psetup->Module = (DWORD)_getModule(CLIENT_DLL_NAME, psetup->procId);
}
BYTE* _getModule(const wchar_t* moduleName, int PID)
{
	HANDLE module = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, PID);
	MODULEENTRY32 moduleEntry;
	moduleEntry.dwSize = sizeof(moduleEntry);

	do
	{
		if (!lstrcmpW(moduleEntry.szModule, moduleName))
		{
			CloseHandle(module);
			return moduleEntry.modBaseAddr;
		}
	} while (Module32Next(module, &moduleEntry));
	CloseHandle(module);
	moduleEntry.modBaseAddr = 0;
	return moduleEntry.modBaseAddr;
}
int getprocId(_In_ wchar_t const* procname)
{
	PROCESSENTRY32 proc_data;
	proc_data.dwSize = sizeof(PROCESSENTRY32);
	HANDLE hndSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	assert(hndSnapshot != INVALID_HANDLE_VALUE && GetLastError() == 0);
	if (Process32First(hndSnapshot, &proc_data))
	{
		if (!lstrcmpW(proc_data.szExeFile, procname))
		{
			return proc_data.th32ProcessID;
		}
		else
		{
			while (Process32Next(hndSnapshot, &proc_data))
			{
				if (!lstrcmpW(proc_data.szExeFile, procname))
				{
					CloseHandle(hndSnapshot);
					return proc_data.th32ProcessID;
				}
			}
		}
	}
	else
	{
		abort();
	}
}
