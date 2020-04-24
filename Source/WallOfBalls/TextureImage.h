#ifndef __TextureImage_h__
#define __TextureImage_h__

#include <windows.h>
#include <gl/gl.h>

typedef struct
{
	GLubyte	*imageData;
	GLuint	bpp;
	GLuint	width;
	GLuint	height;
	GLuint	texID;
} TextureImage;	

#endif