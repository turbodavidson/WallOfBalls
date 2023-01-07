#include "System.h"

bool Dispatch(int);

System S;

void System::Init(void)
{
	if (false == InitWindow(WindowWidth, WindowHeight, WindowBitsPP, WindowFullScreen))
		return; //To Output Device

	if (false == InitGraphicsDriver(_Window, WindowBitsPP))
		return;

	for (int i = 0;i < 256;i++)
		KeyMessage[i] = i;

	for (int counter = 0;counter < 32;counter++) 
		cmdBuffer[counter] = 0;

	_ContentPath = new Path();
	_ContentPath->SetBaseDir(Path::GetParentDirectory(_ContentPath->GetBaseDir()));
	_ContentPath->SetBaseDir(Path::Combine(_ContentPath->GetBaseDir(), "Content"));

	_Resources = new Resources(_ContentPath);
	_Resources->Init();

	_Window->Show();
}

void System::Free(void)
{
	if (false == FreeWindow()) {
		return; //To Output Device
	}

	if (false == FreeGraphicsDriver()) {
		return;
	}

	if (false == FreeResources()) {
		return;
	}

	delete _ContentPath;
	delete _Resources;
}

bool System::InitWindow(int width, int height, int bpp, bool fullScreen)
{
	_Window = new Window();
	return _Window->Init(WindowWidth, WindowHeight, WindowBitsPP, WindowFullScreen);
}

bool System::FreeWindow(void)
{
	bool retVal = _Window->Free();
	delete _Window;
	_Window = 0;
	return retVal;
}

bool System::InitGraphicsDriver(Window* window, int bpp)
{
	_Driver = new DriverGL11(WindowBitsPP);
	return _Driver->Init(window);
}

bool System::FreeGraphicsDriver(void)
{
	bool retVal = _Driver->FreeDevice();
	delete _Driver;
	_Driver = 0;
	return retVal;
}

bool System::InitResources(void)
{
	_Resources = new Resources(_ContentPath);
	return _Resources->Init();
}

bool System::FreeResources(void)
{
	bool retVal = _Resources->Free();
	delete _Resources;
	_Resources = 0;
	return retVal;
}

HWND System::tmpGetWindowHandle(void)
{
	return _Window->GetWindowHandle();
}

ResourceType *System::GetResource(int tag) 
{ 
	return _Resources->GetResource(tag); 
}

void System::ProcMessages(void)
{
	static MSG msg;
	while(PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	for (int i = 0; i < 16; i++)
		Dispatch(cmdBuffer[i]);
}

bool System::Bind(unsigned int key, unsigned int messageToBind)
{
	KeyMessage[key] = messageToBind;
	return true;
}

bool System::AddCommand(int message)
{
	bool hit = false;
	for (int i = 0; i < 16; i++)
	{
		if (message == cmdBuffer[i])
			hit = true;
	}

	if (hit == false)
	{
		for (int i = 0; i < 16; i++)
		{
			if (cmdBuffer[i] == 0)
			{
				cmdBuffer[i] = message;
				break;
			}
		}
	}
	return true;
}

bool System::RemoveCommand(int message)
{
	for (int i = 0; i < 16; i++)
	{
		if (cmdBuffer[i] == message)
		{
			cmdBuffer[i] = 0;
			break;
		}
	}
	return true;
}

bool System::ForceKey(int message, bool value)
{
	for (int i = 0; i < 256; i++)
	{
		if (KeyMessage[i] == message)
			Keys[i] = value;
	}
	return true;
}

vect2_t System::GetPrimaryMonitorResolution()
{
	return {
		(double)GetSystemMetrics(SM_CXSCREEN),
		(double)GetSystemMetrics(SM_CYSCREEN)
	};
}