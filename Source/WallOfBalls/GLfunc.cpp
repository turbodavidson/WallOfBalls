// GLfunc.cpp
#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>

//Function Declarations
/////////////////////////////////////////////////////////////////////////////////////////
LRESULT CALLBACK WndProc(HWND hWnd,UINT uMsg,WPARAM	wParam,LPARAM lParam);
/////////////////////////////////////////////////////////////////////////////////////////

//Variable declarations
/////////////////////////////////////////////////////////////////////////////////////////
extern HGLRC	    hRC;
extern HDC			hDC;
extern HWND			hWnd;
extern HINSTANCE	hInstance;

extern bool app_StillRunning;
extern bool app_Active;
extern bool app_Fullscreen;

extern const GLuint MAX_SCREENMODES;
extern const GLuint MAX_SCREENBPP;

extern GLuint app_ScreenModes[3][2];
extern GLuint app_ScreenBPP[2];

extern GLuint CURRENT_SCREENMODE;
extern GLuint CURRENT_SCREENBPP;
/////////////////////////////////////////////////////////////////////////////////////////

bool InitGL(GLvoid)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);
	glClearDepth(1000.0);
	glClearColor(0.0,0.0,0.0,1.0f);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	return true;
}

GLvoid ReSizeGLScene(GLsizei width, GLsizei height)
{
 if (height==0)
 {
  height=1;
 }
 glViewport (0,0,width,height);
 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();
 gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,4000.0f);
 glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();
}


GLvoid KillGLWindow(GLvoid)
{
 if (app_Fullscreen)
 {
  ChangeDisplaySettings(NULL,0);
  ShowCursor(true);
 }
 if (hRC)
 {
  if (!wglMakeCurrent(NULL,NULL))
  {
   MessageBox(NULL,"Release of DC and RC failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
  }
  if (!wglDeleteContext(hRC))
  {
   MessageBox(NULL,"Release Rendering Context Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
  }
  hRC=NULL;
 }
 if (hDC && !ReleaseDC(hWnd,hDC))
 {
  MessageBox(NULL,"Release of Device Contect Failed.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
  hDC=NULL;
 }
 if (hWnd && !DestroyWindow(hWnd))
 {
  MessageBox(NULL,"Could Not Release hWnd.","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
  hWnd=NULL;
 }
 if (!UnregisterClass("OpenGL",hInstance))
 {
  MessageBox(NULL,"Could Not Unregister Class","SHUTDOWN ERROR",MB_OK | MB_ICONINFORMATION);
  hInstance=NULL;
 }
}

bool CreateGLWindow(char* title,int width,int height,int bits, bool FullScreenflag)
{
 GLuint          PixelFormat;
 WNDCLASS        wc;
 DWORD           dwExStyle;
 DWORD           dwStyle;
 RECT            WindowRect;
 WindowRect.left         = (long)0;
 WindowRect.right        = (long)width;
 WindowRect.top          = (long)0;
 WindowRect.bottom       = (long)height;
 app_Fullscreen              = FullScreenflag;
 hInstance               = GetModuleHandle(NULL);
 wc.style                = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
 wc.lpfnWndProc          = (WNDPROC) WndProc;
 wc.cbClsExtra           = 0;
 wc.cbWndExtra           = 0;
 wc.hInstance            = hInstance;
 wc.hIcon                = LoadIcon(NULL, IDI_WINLOGO);
 wc.hCursor              = LoadCursor(NULL, IDC_ARROW);
 wc.hbrBackground        = NULL;
 wc.lpszMenuName         = NULL;
 wc.lpszClassName        = "OpenGL";
 if (!RegisterClass(&wc))
 {
  MessageBox(NULL,"Failed To Register The Window Class","ERROR",MB_OK | MB_ICONINFORMATION);
  return false;
 }
 if (app_Fullscreen)
 {
  DEVMODE dmScreenSettings;
  memset (&dmScreenSettings,0,sizeof(dmScreenSettings));
  dmScreenSettings.dmSize=sizeof(dmScreenSettings);
  dmScreenSettings.dmPelsWidth=width;
  dmScreenSettings.dmPelsHeight=height;
  dmScreenSettings.dmBitsPerPel=bits;
  dmScreenSettings.dmFields=DM_BITSPERPEL | DM_PELSWIDTH | DM_PELSHEIGHT;
  if (ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)!=DISP_CHANGE_SUCCESSFUL)
  {
   if (MessageBox(NULL,"The Requested FullScreen Mode Is Not Supported By\nYour Video Card. Use Windowed Mode Instead?","Project Nyte Error",MB_YESNO|MB_ICONEXCLAMATION)==IDYES)
   {
    app_Fullscreen=false;
   }
   else
   {
    MessageBox(NULL,"The program will now close","ERROR",MB_OK|MB_ICONSTOP);
    return false;
   }
  }
 }
 if (app_Fullscreen)
 {
  dwExStyle=WS_EX_APPWINDOW;
  dwStyle=WS_POPUP;
  ShowCursor(false);
 }
 else
 {
  dwExStyle=WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
  dwStyle=WS_OVERLAPPEDWINDOW;
 }
 AdjustWindowRectEx(&WindowRect,dwStyle,false,dwExStyle);
 if (!(hWnd=CreateWindowEx(dwExStyle,
                           "OpenGL",
                           title,
                           WS_CLIPSIBLINGS |
                           WS_CLIPCHILDREN |
                           dwStyle,
                           0, 0,
                           WindowRect.right-WindowRect.left,
                           WindowRect.bottom-WindowRect.top,
                           NULL,
                           NULL,
                           hInstance,
                           NULL)))
 {
  KillGLWindow();
  MessageBox(NULL,"Window creation error","ERROR",MB_OK|MB_ICONEXCLAMATION);
  return false;
 }
 static PIXELFORMATDESCRIPTOR pfd=
 {
  sizeof(PIXELFORMATDESCRIPTOR),
  1,
  PFD_DRAW_TO_WINDOW |
  PFD_SUPPORT_OPENGL |
  PFD_DOUBLEBUFFER,
  PFD_TYPE_RGBA,
  bits,
  0,0,0,0,0,0,
  0,
  0,
  0,
  0,0,0,0,
  24,
  0,
  0,
  PFD_MAIN_PLANE,
  0,
  0,0,0,
 };
 if (!(hDC=GetDC(hWnd)))
 {
  KillGLWindow();
  MessageBox(NULL,"Can't Create A GL Device Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
  return false;
 }
 if (!(PixelFormat=ChoosePixelFormat(hDC,&pfd)))
 {
  KillGLWindow();
  MessageBox(NULL,"Can't Find A Suitable PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
  return false;
 }
 if(!SetPixelFormat(hDC,PixelFormat,&pfd))
 {
  KillGLWindow();
  MessageBox(NULL,"Can't Set The PixelFormat.","ERROR",MB_OK|MB_ICONEXCLAMATION);
  return false;
 }
 if (!(hRC=wglCreateContext(hDC)))
 {
  KillGLWindow();
  MessageBox(NULL,"Can't Create A GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
  return false;
 }
 if(!wglMakeCurrent(hDC,hRC))
 {
  KillGLWindow();
  MessageBox(NULL,"Can't Activate The GL Rendering Context.","ERROR",MB_OK|MB_ICONEXCLAMATION);
  return false;
 }
 ShowWindow(hWnd,SW_SHOW);
 SetForegroundWindow(hWnd);
 SetFocus(hWnd);
 ReSizeGLScene(width, height);
 if (!InitGL())
 {
  KillGLWindow();
  MessageBox(NULL,"Initialization Failed.","ERROR",MB_OK|MB_ICONEXCLAMATION);
  return false;
 }
 return true;
}