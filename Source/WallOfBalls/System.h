#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "Window.h"
#include "Driver.h"
#include "DriverGL11.h"
#include "Path.h"
#include "Resources.h"
#include "Interop.h"

class System
{
private:
	Window* _Window;
	DriverGL11* _Driver;
	Path* _ContentPath;
	Resources* _Resources;
	unsigned int cmdBuffer[32];

public:
	bool Keys[256];
	unsigned int KeyMessage[256];

public:
	int WindowWidth;
	int WindowHeight;
	int WindowBitsPP;
	int WindowFullScreen;

public:
	System() { WindowWidth = 1024; WindowHeight = 768; WindowBitsPP = 32; WindowFullScreen = false; };
	~System() { };

	void Init(void);
	void Free(void);
	void ProcMessages(void);
	HWND tmpGetWindowHandle(void);
	DriverGL11 *GetGL11Driver(void) { return _Driver; }
	ResourceType *GetResource(int tag);
	

private:
	bool InitWindow(int width, int height, int bpp, bool fullScreen);
	bool FreeWindow(void);
	bool InitGraphicsDriver(Window* window, int bpp);
	bool FreeGraphicsDriver(void);
	bool InitResources(void);
	bool FreeResources(void);

public:
	bool Bind(unsigned int key, unsigned int messageToBind);
	bool AddCommand(int message);
	bool RemoveCommand(int message);
	bool ForceKey(int message, bool value);
};

extern System S;

#endif