#include <stdio.h>
#include <windows.h>
#include <assert.h>
#include "cheat_launcher.h"
#include "player.h"
#include "offsets.h"
int main()
{
	struct colors enemy = {  1.f,0.f,0.7f };
	init_default_cheat_opts();
	start(&enemy);
	return 0;
}