#ifndef __WINDOW_H
#define __WINDOW_H

#include <Windows.h>

class Window
{
private:
	HINSTANCE _hInstance;
	LPCWSTR _ClassName;
	int _Width;
	int _Height;
	int _winWidth;
	int _winHeight;
	int _BitsPP;
	bool _IsFullScreen;
	HWND _WindowHandle;

public:
	Window() { _ClassName = L"WallOfBalls"; };
	~Window() { };

	bool Init(int width, int height, int bpp, bool fullScreen);
	bool Free(void);
	bool Show(void);
	bool IsFullScreen(void);
	HWND GetWindowHandle(void);
};

#endif