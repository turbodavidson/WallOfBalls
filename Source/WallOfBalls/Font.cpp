#include	<windows.h>										// Header File For Windows
#include	<stdio.h>										// Header File For Standard Input / Output
#include	<stdarg.h>										// Header File For Variable Argument Routines
#include	<string.h>										// Header File For String Management
#include	<gl\gl.h>										// Header File For The OpenGL32 Library
#include	<gl\glu.h>	
#include "TextureImage.h"

extern TextureImage Fonts[1];
extern GLuint base;

extern GLuint app_ScreenModes[3][2];

extern GLuint app_ScreenBPP[2];

extern GLuint CURRENT_SCREENMODE;
extern GLuint CURRENT_SCREENBPP;

GLvoid BuildFont(GLvoid)									// Build Our Font Display List
{
	base=glGenLists(256);									// Creating 256 Display Lists
	glBindTexture(GL_TEXTURE_2D, Fonts[0].texID);		// Select Our Font Texture
	for (int loop1=0; loop1<256; loop1++)					// Loop Through All 256 Lists
	{
		float cx=float(loop1%16)/16.0f;						// X Position Of Current Character
		float cy=float(loop1/16)/16.0f;						// Y Position Of Current Character

		glNewList(base+loop1,GL_COMPILE);					// Start Building A List
			glBegin(GL_QUADS);								// Use A Quad For Each Character
				glTexCoord2f(cx,1.0f-cy-0.0625f);			// Texture Coord (Bottom Left)
				glVertex2d(0,16);							// Vertex Coord (Bottom Left)
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.0625f);	// Texture Coord (Bottom Right)
				glVertex2i(16,16);							// Vertex Coord (Bottom Right)
				glTexCoord2f(cx+0.0625f,1.0f-cy-0.001f);	// Texture Coord (Top Right)
				glVertex2i(16,0);							// Vertex Coord (Top Right)
				glTexCoord2f(cx,1.0f-cy-0.001f);			// Texture Coord (Top Left)
				glVertex2i(0,0);							// Vertex Coord (Top Left)
			glEnd();										// Done Building Our Quad (Character)
			glTranslated(14,0,0);							// Move To The Right Of The Character
		glEndList();										// Done Building The Display List
	}														// Loop Until All 256 Are Built
}

GLvoid KillFont(GLvoid)										// Delete The Font From Memory
{
	glDeleteLists(base,256);								// Delete All 256 Display Lists
}


GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...)	// Where The Printing Happens
{
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D,Fonts[0].texID);
	glViewport (0,0,app_ScreenModes[CURRENT_SCREENMODE][0],app_ScreenModes[CURRENT_SCREENMODE][1]);
	glMatrixMode(GL_PROJECTION);							// Select The Projection Matrix
	glLoadIdentity();										// Reset The Projection Matrix
	glOrtho(0.0f,app_ScreenModes[CURRENT_SCREENMODE][0],app_ScreenModes[CURRENT_SCREENMODE][1],0.0f,-1.0f,1.0f);					// Create Ortho 640x480 View (0,0 At Top Left)
	glMatrixMode(GL_MODELVIEW);								// Select The Modelview Matrix
	glLoadIdentity();										// Reset The Modelview Matrix
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

	glEnable(GL_TEXTURE_2D);								// Enable Texture Mapping
	glLoadIdentity();										// Reset The Modelview Matrix
	glTranslated(x,y,0);									// Position The Text (0,0 - Top Left)
	glListBase(base-32+(128*set));							// Choose The Font Set (0 or 1)

	glScalef(1.0f,2.0f,1.0f);								// Make The Text 2X Taller

	glCallLists(strlen(text),GL_UNSIGNED_BYTE, text);		// Write The Text To The Screen
	//glDisable(GL_TEXTURE_2D);								// Disable Texture Mapping
	glViewport (0,0,app_ScreenModes[CURRENT_SCREENMODE][0],app_ScreenModes[CURRENT_SCREENMODE][1]);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,(GLfloat)app_ScreenModes[CURRENT_SCREENMODE][0]/(GLfloat)app_ScreenModes[CURRENT_SCREENMODE][1],0.1f,4000.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPopMatrix();
}