// WndProc.cpp
#include <windows.h>
#include <gl/gl.h>

#include "GlobalEnum.h"
#include "Camera.h"

//Function Declarations
/////////////////////////////////////////////////////////////////////////////////////////
GLvoid ReSizeGLScene(GLsizei width, GLsizei height);
bool Dispatch(int);
/////////////////////////////////////////////////////////////////////////////////////////

//Variable declarations
/////////////////////////////////////////////////////////////////////////////////////////
extern HGLRC	    hRC;
extern HDC			hDC;
extern HWND			hWnd;
extern HINSTANCE	hInstance;

extern GLuint KeyMessage[256];
extern bool Keys[256];

extern bool app_StillRunning;
extern bool app_Active;
extern bool app_Fullscreen;

extern Camera *DefaultView;

extern int mouseprevx;
extern int mouseprevy;
/////////////////////////////////////////////////////////////////////////////////////////

bool AddCommand(int);
bool RemoveCommand(int);

LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM	wParam,LPARAM lParam)
{
 switch (uMsg)
 {
  case WM_ACTIVATE:
  {
   if (!HIWORD(wParam))
   {
    app_Active=true;
   }
   else
   {
    app_Active=false;
   }
   return 0;
  }
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
  case WM_CLOSE:
  {
   PostQuitMessage(0);
   return 0;
  }
  case WM_SIZE:
  {
   ReSizeGLScene(LOWORD(lParam),HIWORD(lParam));
   return 0;
  }
  case WM_KEYDOWN:
  {
	  if (Keys[wParam]==false)
	  {
		  Keys[wParam]=true;
		  AddCommand(KeyMessage[wParam]);
	  }
   return 0;
  }
  case WM_KEYUP:
  {
	  if (Keys[wParam])
	  {
		  Keys[wParam]=false;
		  RemoveCommand(KeyMessage[wParam]);
	  }

   return 0;
  }
  case WM_MOUSEMOVE:
	  {
		  DefaultView->LookSideWays(-(double(mouseprevx-(LOWORD(lParam))))/8.0);
		  mouseprevx=LOWORD(lParam);
		  DefaultView->LookUp(-(double(mouseprevy-(HIWORD(lParam))))/8.0);
		  mouseprevy=HIWORD(lParam);

		  if (LOWORD(lParam)<312)
		  {
			  SetCursorPos(512,384);
			  mouseprevx=512;
			  mouseprevy=384;
		  }
		  if (LOWORD(lParam)>712)
		  {
			  SetCursorPos(512,384);
			  mouseprevx=512;
			  mouseprevy=384;
		  }
		  if (HIWORD(lParam)<184)
		  {
			  SetCursorPos(512,384);
			  mouseprevx=512;
			  mouseprevy=384;
		  }
		  if (HIWORD(lParam)>584)
		  {
			  SetCursorPos(512,384);
			  mouseprevx=512;
			  mouseprevy=384;
		  }
	  return 0;
	  }
  case WM_LBUTTONDOWN:
	  {
		  AddCommand(KEY_FIREGUN);
		  return 0;
	  }
  case WM_LBUTTONUP:
	  {
		  RemoveCommand(KEY_FIREGUN);
		  return 0;
	  }
 }
 return DefWindowProc(hWnd,uMsg,wParam,lParam);
}
