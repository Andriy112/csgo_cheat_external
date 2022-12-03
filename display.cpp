#include "display.h"
#include <cassert>
#include "windows.h"
display display::current_display()
{
	DEVMODE devMode;
	DISPLAY_DEVICE displayDevice = { .cb = sizeof(displayDevice) };
	ZeroMemory(&devMode, sizeof(DEVMODE));
	devMode.dmSize = sizeof(DEVMODE);

	assert(EnumDisplayDevices(NULL, 0, &displayDevice, EDD_GET_DEVICE_INTERFACE_NAME));

	assert(EnumDisplaySettings(displayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &devMode));

	return display{ .height = devMode.dmPelsHeight,.width = devMode.dmPelsWidth };
}

