#ifndef __DRIVER_H
#define __DRIVER_H

#include "Window.h"

class Driver
{
public:
	Driver();

	virtual bool Init(Window* window) = 0;
	virtual bool Free(void) = 0;
	virtual bool InitDevice(void) = 0;
	virtual bool FreeDevice(void) = 0;
};

#endif