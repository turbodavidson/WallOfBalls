#include	<windows.h>										// Header File For Windows
#include	<stdio.h>										// Header File For Standard Input / Output
#include	<stdarg.h>										// Header File For Variable Argument Routines
#include	<string.h>										// Header File For String Management
#include	<gl\gl.h>										// Header File For The OpenGL32 Library
#include	<gl\glu.h>	

#include "TextureImage.h"

#include <stb_image.h>	

bool LoadTGA(TextureImage *texture, char *filename, bool invertY)						// Loads A TGA File Into Memory
{    
	int width, height, channels;
	stbi_set_flip_vertically_on_load(invertY);
	unsigned char *imageData = stbi_load(filename, &width, &height, &channels, 0);
	if (imageData) {
		int imageDataSize = width * height * channels;
		texture->width = width;
		texture->height = height;
		texture->bpp = channels * 8;
		texture->imageData = (GLubyte*)malloc(imageDataSize);
		memcpy(texture->imageData, imageData, imageDataSize);
		stbi_image_free(imageData);

		// Build A Texture From The Data
		glGenTextures(1, &texture->texID);						// Generate OpenGL texture IDs

		glBindTexture(GL_TEXTURE_2D, texture->texID);			// Bind Our Texture
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	// Linear Filtered
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	// Linear Filtered

		GLint glPixelFormat = texture->bpp == 24 ? GL_RGB : GL_RGBA;

		glTexImage2D(GL_TEXTURE_2D, 0, glPixelFormat, texture->width, texture->height, 0, glPixelFormat, GL_UNSIGNED_BYTE, texture->imageData);

		return true;											// Texture Building Went Ok, Return True
	}
	else {
		return false;
	}
}