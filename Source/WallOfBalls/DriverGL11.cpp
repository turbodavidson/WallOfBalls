#include <stdio.h>
#include "DriverGL11.h"

#include "Game.h"

DriverGL11::DriverGL11(int windowBitsPP)
{
	_lib = NULL;
	_WindowBitsPP = windowBitsPP;
}

bool DriverGL11::Init(Window* window)
{
	_Window = window;
	LoadLib();
	return InitDevice();
}

bool DriverGL11::Free(void)
{
	FreeLib();
	return FreeDevice();
}

bool DriverGL11::InitDevice()
{
	if (!(_HDC = GetDC(_Window->GetWindowHandle())))
		return false;

	PIXELFORMATDESCRIPTOR pfd = {
		sizeof(PIXELFORMATDESCRIPTOR),  // size of structure
		1,                              // default version
		PFD_DRAW_TO_WINDOW |            // window drawing support
		PFD_SUPPORT_OPENGL |            // OpenGL support
		PFD_DOUBLEBUFFER,               // double buffering support
		PFD_TYPE_RGBA,                  // RGBA color mode
		_WindowBitsPP,                  // 32 bit color mode
		0, 0, 0, 0, 0, 0,               // ignore color bits, non-palettized mode
		0,                              // no alpha buffer
		0,                              // ignore shift bit
		0,                              // no accumulation buffer
		0, 0, 0, 0,                     // ignore accumulation bits
		24,                             // 16 bit z-buffer size
		0,                              // no stencil buffer
		0,                              // no auxiliary buffer
		PFD_MAIN_PLANE,                 // main drawing plane
		0,                              // reserved
		0, 0, 0 };                      // layer masks ignored

	GLuint pixelFormat;

	if (!(pixelFormat = ChoosePixelFormat(_HDC, &pfd)))
		return false;

	if(!SetPixelFormat(_HDC, pixelFormat, &pfd))
		return false;

	if (!(_HRC = wglCreateContext(_HDC)))
		return false;
	
	if(!wglMakeCurrent(_HDC, _HRC))
		return false;

	return true;
}

bool DriverGL11::FreeDevice()
{
	if (_HRC)
	{
		wglMakeCurrent(_HDC, 0);
		wglDeleteContext(_HRC);
		_HRC = 0;
		_HDC = 0;
	}
	return true;
}

void DriverGL11::ReSizeGLScene(int width, int height)
{
	if (DriverGL11::Viewport == NULL) {
		return;
	}

	if (height == 0)
	{
		height = 1;
	}

	DriverGL11::Viewport (0, 0, width, height);
	DriverGL11::MatrixMode(GL_PROJECTION);
	DriverGL11::LoadIdentity();
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 4000.0f);
	DriverGL11::MatrixMode(GL_MODELVIEW);
	DriverGL11::LoadIdentity();
}

void DriverGL11::BuildFont(void)									// Build Our Font Display List
{
	FontBase = DriverGL11::GenLists(256);									// Creating 256 Display Lists
	DriverGL11::BindTexture(GL_TEXTURE_2D, G.font.texId);		// Select Our Font Texture
	for (int loop1=0; loop1<256; loop1++)					// Loop Through All 256 Lists
	{
		float cx=float(loop1%16)/16.0f;						// X Position Of Current Character
		float cy=float(loop1/16)/16.0f;						// Y Position Of Current Character

		DriverGL11::NewList(FontBase+loop1,GL_COMPILE);					// Start Building A List
			DriverGL11::Begin(GL_QUADS);								// Use A Quad For Each Character
				DriverGL11::TexCoord2f(cx,1.0f-cy-0.0625f);			// Texture Coord (Bottom Left)
				DriverGL11::Vertex2d(0,16);							// Vertex Coord (Bottom Left)
				DriverGL11::TexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord (Bottom Right)
				DriverGL11::Vertex2i(16,16);							// Vertex Coord (Bottom Right)
				DriverGL11::TexCoord2f(cx+0.0625f,1.0f-cy-0.001f);	// Texture Coord (Top Right)
				DriverGL11::Vertex2i(16,0);							// Vertex Coord (Top Right)
				DriverGL11::TexCoord2f(cx,1.0f-cy-0.001f);			// Texture Coord (Top Left)
				DriverGL11::Vertex2i(0,0);							// Vertex Coord (Top Left)
			DriverGL11::End();										// Done Building Our Quad (Character)
			DriverGL11::Translated(14,0,0);							// Move To The Right Of The Character
		DriverGL11::EndList();										// Done Building The Display List
	}														// Loop Until All 256 Are Built
}

void DriverGL11::KilFont(void)										// Delete The Font From Memory
{
	DriverGL11::DeleteLists(FontBase, 256);								// Delete All 256 Display Lists
}


void DriverGL11::Print(GLint x, GLint y, int set, const char* fmt, ...)	// Where The Printing Happens
{
	float scaleFactor = S.WindowHeight / 480.0f;
	vect2_t canvasSize = { scaleFactor * 640.0f, S.WindowHeight };
	float sideBarWidth = (S.WindowWidth - canvasSize.x) / 2.0f;
	vect2_t topLeft = { sideBarWidth, 0.0f };

	DriverGL11::PushMatrix();
	DriverGL11::BindTexture(GL_TEXTURE_2D, G.font.texId);
	DriverGL11::Viewport(topLeft.x, topLeft.y, canvasSize.x, canvasSize.y);
	DriverGL11::MatrixMode(GL_PROJECTION);
	DriverGL11::LoadIdentity();
	DriverGL11::Ortho(0.0f, 1024.0f, 768.0f, 0.0f, -1.0f, 1.0f);
	DriverGL11::MatrixMode(GL_MODELVIEW);
	DriverGL11::LoadIdentity();
	char		text[1024];									// Holds Our String
	va_list		ap;											// Pointer To List Of Arguments

	if (fmt == NULL)										// If There's No Text
		return;												// Do Nothing

	va_start(ap, fmt);										// Parses The String For Variables
	vsprintf(text, fmt, ap);							// And Converts Symbols To Actual Numbers
	va_end(ap);												// Results Are Stored In Text

	if (set>1)												// Did User Choose An Invalid Character Set?
	{
		set=1;												// If So, Select Set 1 (Italic)
	}

	DriverGL11::Enable(GL_TEXTURE_2D);								// Enable Texture Mapping
	DriverGL11::LoadIdentity();										// Reset The Modelview Matrix
	DriverGL11::Translated(x,y,0);									// Position The Text (0,0 - Top Left)
	DriverGL11::ListBase(FontBase-31+(128*set));							// Choose The Font Set (0 or 1)

	DriverGL11::Scalef(1.0f,2.0f,1.0f);								// Make The Text 2X Taller

	DriverGL11::CallLists(strlen(text),GL_UNSIGNED_BYTE, text);		// Write The Text To The Screen
	//glDisable(GL_TEXTURE_2D);								// Disable Texture Mapping
	DriverGL11::Viewport(topLeft.x, topLeft.y, canvasSize.x, canvasSize.y);
	DriverGL11::MatrixMode(GL_PROJECTION);
	DriverGL11::LoadIdentity();
	gluPerspective(45.0f, 640.0f/480.0f, 0.1f, 4000.0f);
	DriverGL11::MatrixMode(GL_MODELVIEW);
	DriverGL11::LoadIdentity();
	DriverGL11::PopMatrix();
}