#include "player.h"
#include <cassert>
#include "windows.h"
#include "offsets.h"
#include <stdio.h>
#include "cheat_launcher.h"
#include "display.h"
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
bool player::fire(int count)
{
	static int fired = 0;
	if (fired > 7)
	{
		fired = 0;
	}
	fired += count;
	if (fired > 7)
	{
		fired = 0;
	}
	Sleep(20);
	assert(WriteProcessMemory(setup.proc, (LPVOID)((DWORD32)setup.Module + dwForceAttack), &fired, 4, 0));
	Sleep(50);
	return true;
}
boneMatrix_t* get_head(player* pplayer) 
{
	uintptr_t bone_address;
	boneMatrix_t *head = (boneMatrix_t*)malloc(sizeof(boneMatrix_t));
	head->x = (float*)malloc(sizeof(float));
	head->y = (float*)malloc(sizeof(float));
	head->z = (float*)malloc(sizeof(float));

	
	assert(ReadProcessMemory(setup.proc,(LPCVOID)((DWORD32)pplayer->address + m_dwBoneMatrix),&bone_address,sizeof bone_address,nullptr));
	//assert(ReadProcessMemory(setup.proc, (LPCVOID)((DWORD32)bone_address+ (8*sizeof(*head))), head, sizeof *head, nullptr));
	assert(ReadProcessMemory(setup.proc, (LPCVOID)((DWORD32)bone_address+ (0X30*8)+0x0C), head->x, sizeof(float), nullptr));
	assert(ReadProcessMemory(setup.proc, (LPCVOID)((DWORD32)bone_address + (0X30 * 8)+0x1C), head->y, sizeof(float), nullptr));
	assert(ReadProcessMemory(setup.proc, (LPCVOID)((DWORD32)bone_address + (0X30 * 8)+0x2C), head->z, sizeof(float), nullptr));

	return head;
}
vec* vec::xyz_to_xy() 
{
	display curr_disp =display::current_display();
	float view_matrix[16];
	unsigned module = setup.Module + dwViewMatrix;
	assert(ReadProcessMemory(setup.proc,(LPCVOID)module,view_matrix,sizeof(view_matrix),nullptr));
	float x = view_matrix[0] * X + view_matrix[1] * Y + view_matrix[2] * Z + view_matrix[3];
	float y = view_matrix[4] * X + view_matrix[5] * Y + view_matrix[6] * Z + view_matrix[7];
	float w  = view_matrix[12] * X + view_matrix[13] * Y + view_matrix[14] * Z + view_matrix[15];
	 
	x *= (1.0f / w);
	y *= (1.0f / w);

	float h =curr_disp.width / 2, wt = curr_disp.height / 2;

	h += 0.5f * x * curr_disp.width + 0.5f;
	wt -= 0.5f * y * curr_disp.height + 0.5f;
	X = h;
	Y = wt;
	return this;
}
int delete_bonematrix(boneMatrix_t *bone) 
{
	delete bone->x;
	delete bone->y;
	delete bone->z;
	delete bone;
	return 0;
}
void read_glwIndex(player * pplayer, UINT* glowIndex)
{
	ReadProcessMemory(setup.proc, (LPCVOID)(((DWORD32)pplayer->address) + m_iGlowIndex), glowIndex, 4, 0);
}
void read_glw_obj_manager(UINT* glowObjManager)
{
	assert(ReadProcessMemory(setup.proc, (LPCVOID)(setup.Module + dwGlowObjectManager), glowObjManager, 4, 0));
}