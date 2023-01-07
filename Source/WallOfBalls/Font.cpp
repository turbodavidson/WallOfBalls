#include	<windows.h>										// Header File For Windows
#include	<stdio.h>										// Header File For Standard Input / Output
#include	<stdarg.h>										// Header File For Variable Argument Routines
#include	<string.h>										// Header File For String Management
#include	<gl\glu.h>	
#include "DriverGL11.h"
#include "Game.h"

static GLuint base;

extern GLuint app_ScreenModes[3][2];

extern GLuint app_ScreenBPP[2];

extern GLuint CURRENT_SCREENMODE;
extern GLuint CURRENT_SCREENBPP;

GLvoid BuildFont(GLvoid)									// Build Our Font Display List
{
	base=DriverGL11::GenLists(256);									// Creating 256 Display Lists
	//DriverGL11::BindTexture(GL_TEXTURE_2D, Fonts[0].texID);		// Select Our Font Texture
	DriverGL11::BindTexture(GL_TEXTURE_2D, G.font.texId);		// Select Our Font Texture
	for (int loop1=0; loop1<256; loop1++)					// Loop Through All 256 Lists
	{
		float cx=float(loop1%16)/16.0f;						// X Position Of Current Character
		float cy=float(loop1/16)/16.0f;						// Y Position Of Current Character

		DriverGL11::NewList(base+loop1,GL_COMPILE);					// Start Building A List
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

GLvoid KillFont(GLvoid)										// Delete The Font From Memory
{
	DriverGL11::DeleteLists(base,256);								// Delete All 256 Display Lists
}


//GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)	// Where The Printing Happens
//{
//	DriverGL11::PushMatrix();
//	//DriverGL11::BindTexture(GL_TEXTURE_2D, Fonts[0].texID);
//	DriverGL11::BindTexture(GL_TEXTURE_2D, G.font.texId);
//	DriverGL11::Viewport (0,0,app_ScreenModes[CURRENT_SCREENMODE][0],app_ScreenModes[CURRENT_SCREENMODE][1]);
//	DriverGL11::MatrixMode(GL_PROJECTION);							// Select The Projection Matrix
//	DriverGL11::LoadIdentity();										// Reset The Projection Matrix
//	DriverGL11::Ortho(0.0f,app_ScreenModes[CURRENT_SCREENMODE][0],app_ScreenModes[CURRENT_SCREENMODE][1],0.0f,-1.0f,1.0f);					// Create Ortho 640x480 View (0,0 At Top Left)
//	DriverGL11::MatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
//	DriverGL11::LoadIdentity();										// Reset The Modelview Matrix
//	char		text[1024];									// Holds Our String
//	va_list		ap;											// Pointer To List Of Arguments
//
//	if (fmt == NULL)										// If There's No Text
//		return;												// Do Nothing
//
//	va_start(ap, fmt);										// Parses The String For Variables
//	    vsprintf(text, fmt, ap);							// And Converts Symbols To Actual Numbers
//	va_end(ap);												// Results Are Stored In Text
//
//	if (set>1)												// Did User Choose An Invalid Character Set?
//	{
//		set=1;												// If So, Select Set 1 (Italic)
//	}
//
//	DriverGL11::Enable(GL_TEXTURE_2D);								// Enable Texture Mapping
//	DriverGL11::LoadIdentity();										// Reset The Modelview Matrix
//	DriverGL11::Translated(x,y,0);									// Position The Text (0,0 - Top Left)
//	DriverGL11::ListBase(base-32+(128*set));							// Choose The Font Set (0 or 1)
//
//	DriverGL11::Scalef(1.0f,2.0f,1.0f);								// Make The Text 2X Taller
//
//	DriverGL11::CallLists(strlen(text),GL_UNSIGNED_BYTE, text);		// Write The Text To The Screen
//	//glDisable(GL_TEXTURE_2D);								// Disable Texture Mapping
//	DriverGL11::Viewport (0,0,app_ScreenModes[CURRENT_SCREENMODE][0],app_ScreenModes[CURRENT_SCREENMODE][1]);
//	DriverGL11::MatrixMode(GL_PROJECTION);
//	DriverGL11::LoadIdentity();
//	gluPerspective(45.0f,(GLfloat)app_ScreenModes[CURRENT_SCREENMODE][0]/(GLfloat)app_ScreenModes[CURRENT_SCREENMODE][1],0.1f,4000.0f);
//	DriverGL11::MatrixMode(GL_MODELVIEW);
//	DriverGL11::LoadIdentity();
//	DriverGL11::PopMatrix();
//}