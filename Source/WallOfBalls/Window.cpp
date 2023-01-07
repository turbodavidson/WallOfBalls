#include "Window.h"

// WndProc.cpp
#include <windows.h>
#include <gl/gl.h>

#include "GlobalEnum.h"
#include "Camera.h"
#include "Game.h"

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SYSCOMMAND:
		{
			switch (wParam)
			{
				case SC_SCREENSAVE:
				case SC_MONITORPOWER:
				return 0;
			}
			break;
		}
		case WM_DESTROY:
		{
			G.app_StillRunning = false;
			return 0;
		}
		case WM_CLOSE:
		{
			G.app_StillRunning = false;
			return 0;
		}
		case WM_SIZE:
		{
			S.GetGL11Driver()->ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
			return 0;
		}
		case WM_KEYDOWN:
		{
			if (S.Keys[wParam] == false)
			{
				S.Keys[wParam] = true;
				S.AddCommand(S.KeyMessage[wParam]);
			}
			return 0;
		}
		case WM_KEYUP:
		{
			if (S.Keys[wParam])
			{
				S.Keys[wParam] = false;
				S.RemoveCommand(S.KeyMessage[wParam]);
			}
			return 0;
		}
		case WM_MOUSEMOVE:
		{
			if (0 != G.DefaultView)
			{
				if (G.DefaultView->Look(LOWORD(lParam), HIWORD(lParam)))
					SetCursorPos(512, 384);

				return 0;
			}
		}
		case WM_LBUTTONDOWN:
		{
			S.AddCommand(KEY_FIREGUN);
			return 0;
		}
		case WM_LBUTTONUP:
		{
			S.RemoveCommand(KEY_FIREGUN);
			return 0;
		}
	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

bool Window::Init(int width, int height, int bpp, bool fullScreen)
{
	_hInstance = GetModuleHandle(NULL);

	WNDCLASSEX windowClass;
	memset(&windowClass, 0, sizeof(WNDCLASSEX));
	windowClass.cbSize			= sizeof(WNDCLASSEX);
	windowClass.style			= CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	windowClass.lpfnWndProc		= WndProc;
	windowClass.cbClsExtra		= 0;
	windowClass.cbWndExtra		= 0;
	windowClass.hInstance		= _hInstance;
	windowClass.hIcon			= LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hIconSm			= LoadIcon(NULL, IDI_WINLOGO);
	windowClass.hCursor			= LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground	= NULL;
	windowClass.lpszMenuName	= NULL;
	windowClass.lpszClassName	= _ClassName;
	
	if (!RegisterClassEx(&windowClass))
		return false;
	
	DWORD windowStyle = WS_OVERLAPPEDWINDOW | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
	DWORD windowStyleEx = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	if (true == fullScreen)
	{
		windowStyle = WS_POPUP | WS_CLIPSIBLINGS | WS_CLIPCHILDREN;
		windowStyleEx = WS_EX_APPWINDOW;
		ShowCursor(false);
	}
	windowStyle &= (~WS_VISIBLE);

	RECT windowRect;
	memset(&windowRect, 0, sizeof(RECT));
	windowRect.left = windowRect.top = 0;
	windowRect.right = (LONG)width;
	windowRect.bottom = (LONG)height;
	AdjustWindowRectEx(&windowRect, windowStyle, FALSE, windowStyleEx);
	_winWidth = windowRect.right - windowRect.left;
	_winHeight = windowRect.bottom - windowRect.top;

	if (false == fullScreen) 
	{
		windowRect.left = (GetSystemMetrics(SM_CXSCREEN) - _winWidth) / 2;
		windowRect.top = (GetSystemMetrics(SM_CYSCREEN) - _winWidth) / 2;
		
		if (windowRect.left < 0) 
			windowRect.left = 0;
		
		if (windowRect.top < 0)
			windowRect.top = 0;

		windowRect.right = windowRect.left + _winWidth;
		windowRect.bottom = windowRect.top + _winHeight;
	}

	DEVMODE screenSettings;
	memset(&screenSettings, 0, sizeof(DEVMODE));
	if (fullScreen) 
	{
		screenSettings.dmSize       = sizeof(DEVMODE);
		screenSettings.dmPelsWidth  = _Width = width;
		screenSettings.dmPelsHeight = _Height = height;
		screenSettings.dmBitsPerPel = _BitsPP = bpp;
		screenSettings.dmFields     = DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
		_IsFullScreen = (DISP_CHANGE_SUCCESSFUL == ChangeDisplaySettings(&screenSettings, CDS_FULLSCREEN));
	}

	_WindowHandle = NULL;
	_WindowHandle = CreateWindowEx(windowStyleEx,
		_ClassName,
		_ClassName,
		windowStyle,
		windowRect.left,
		windowRect.top, 
		windowRect.right - windowRect.left,
		windowRect.bottom - windowRect.top,
		NULL,
		NULL,
		_hInstance,
		NULL);

	return (false == _WindowHandle) ? 0 : 1;
}

bool Window::Free(void)
{
	bool retVal = true;
	if (_WindowHandle && !DestroyWindow(_WindowHandle))
		retVal = false;

	_WindowHandle = 0;
	
	if (!UnregisterClass(_ClassName, _hInstance))
		retVal = false;
	
	_hInstance = 0;
	
	return retVal;
}

HWND Window::GetWindowHandle(void)
{
	return _WindowHandle;
}

bool Window::Show(void)
{
	ShowWindow(_WindowHandle, SW_SHOW);
	SetForegroundWindow(_WindowHandle);
	SetFocus(_WindowHandle);
	return true;
}