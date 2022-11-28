//#include "player.h"

struct csgo_setup
{
	//proccess id, module address
	unsigned procId, Module;
	void* proc;
}/*base instance to set write/read options for csgo process*/ setup;
 void set_options(struct csgo_setup* psetup);
//Should use this function before call start.
#define init_default_cheat_opts() set_options(&setup)
 int start(struct colors* pcolors);
 int getprocId(_In_ wchar_t const* procname);
 BYTE* _getModule(const wchar_t* moduleName, int PID);


