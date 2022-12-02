#include <assert.h>
#include "offsets.h"
#include "windows.h"
#include <tlhelp32.h>
#include <stdbool.h>
#include "player.h"
#include "cheat_launcher.h"

int start(struct colors* pcolors)
{
	boneMatrix_t* head =nullptr;
	vec xy;
	player* me = get_next_entity_obj(0);
	DWORD32 glowObjManager, glowIndex;
#pragma region trying to read glowObjectManager
	assert(ReadProcessMemory(setup.proc, (LPCVOID)(setup.Module + dwGlowObjectManager), &glowObjManager, 4, 0));
#pragma endregion
	player* __player;
	for (;;)
	{
		for (size_t i = 1; i < 64; i++)
		{
			__player = get_next_entity_obj(i);

			//returns iterated player's glowIndex
			ReadProcessMemory(setup.proc, (LPCVOID)(((DWORD32)__player->address) + m_iGlowIndex), &glowIndex, 4, 0);
			if (__player->teamNum != me->teamNum)
			{
				if (__player->address)
				{
					head = get_head(__player);
					xy.X = *head->x;
					xy.Y = *head->y;
					xy.Z = *head->z;
					delete_bonematrix(head);
					xy.xyz_to_xy();
				}
				put_wallhack(&glowObjManager, &glowIndex, pcolors);
				/*if center of display == head_position ,then fire */
				if (xy.X > 950 && xy.X < 970 && xy.Y > 540 && xy.Y < 560)
				{
					me->fire(3);
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
	return 1;
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

	return 0;
}
display display::current_display() 
{
	DEVMODE devMode;
	DISPLAY_DEVICE displayDevice = { .cb = sizeof(displayDevice) };
	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize = sizeof(DEVMODE);

	assert(EnumDisplayDevices(NULL, 0, &displayDevice, EDD_GET_DEVICE_INTERFACE_NAME));

	assert(EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode));

	return display{.height =devMode.dmPelsHeight,.width = devMode.dmPelsWidth};
}
