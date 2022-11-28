#include "windows.h"
struct colors
{
	float r, g, b;
};

class player
{
public:
	//Address of current player
	LPVOID address;
	/// <summary>
	/// Should be 2 or 3
	/// </summary>
	unsigned short teamNum;
	unsigned short health;
	bool fire();
	/*Method on_enemy is available only playing with bots,
	 because i cannot receive id of player in official matches.*/
	virtual bool on_enemy();
};

player* get_next_entity_obj(int index);
void put_wallhack(DWORD32* glwobj, DWORD32* glwInd, struct colors const* c);

