#include "player.h"
#include <cassert>
#include "windows.h"
#include "offsets.h"
#include "cheat_launcher.h"

player* get_next_entity_obj(int index)
{
	player* p = new player;
	int players_count = 0;
	int entity_address = 0, localplayer_hp = 0, team_num = 0;
	assert(ReadProcessMemory(setup.proc, (LPCVOID)(setup.Module + dwEntityList + index * 0x10), &entity_address, 4, 0));
	ReadProcessMemory(setup.proc, (LPCVOID)(entity_address + dwHealth), &localplayer_hp, 4, 0);
	ReadProcessMemory(setup.proc, (LPCVOID)(entity_address + m_iTeamNum), &team_num, 4, 0);
	p->address = (LPVOID)entity_address;
	p->teamNum = team_num;
	p->health = localplayer_hp;
	return p;
}
VOID put_wallhack(DWORD32* glwobj, DWORD32* glwInd, struct colors const* c)
{
	bool enable = true;
	static float a = 0.5f;
	assert(WriteProcessMemory(setup.proc, (LPVOID)(*glwobj + *glwInd * 0x38 + 0x8), &c->r, sizeof(float), 0));
	assert(WriteProcessMemory(setup.proc, (LPVOID)(*glwobj + *glwInd * 0x38 + 0xC), &c->g, sizeof(float), 0));
	assert(WriteProcessMemory(setup.proc, (LPVOID)(*glwobj + *glwInd * 0x38 + 0x10), &c->b, sizeof(float), 0));
	assert(WriteProcessMemory(setup.proc, (LPVOID)(*glwobj + *glwInd * 0x38 + 0x14), &a, sizeof(float), 0));
	assert(WriteProcessMemory(setup.proc, (LPVOID)(*glwobj + *glwInd * 0x38 + 0x28), &enable, 4, 0));
}
bool player::fire()
{
	static int fired = 0;

	fired += 25;
	assert(WriteProcessMemory(setup.proc, (LPVOID)((DWORD32)setup.Module + dwForceAttack), &fired, 4, 0));

	return true;
}
bool player::on_enemy()
{
	int id;
	player* enemy = nullptr;
	
	/// <summary>
	/// </summary>
	/// <returns>entity id of pointed player</returns>
	assert(ReadProcessMemory(setup.proc, (LPCVOID)((DWORD32)address + m_iCrosshairId), &id, 4, nullptr));

	enemy = get_next_entity_obj(id);
	if (id < 64 && enemy && id != 0/* && enemy->teamNum != teamNum*/)
	{
		delete enemy;
		return true;
	}
	delete enemy;
	return false;
}
