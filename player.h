#include "windows.h"
struct colors
{
	float r, g, b;
};
//my vector struct
struct vec
{
	FLOAT X,Y,Z;		
	vec* xyz_to_xy();
};
struct boneMatrix_t {
	float *x,*y,*z;
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
	bool fire(int);
};

player* get_next_entity_obj(int index);
void put_wallhack(DWORD32* glwobj, DWORD32* glwInd, struct colors const* c);
boneMatrix_t* get_head(player *pplayer);
int delete_bonematrix(boneMatrix_t*);
void read_glwIndex(player * , DWORD32* );
void read_glw_obj_manager(UINT*);

