#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "gl/glaux.h"
#include <stdio.h>

#include "TextureImage.h"
#include "LoadTGA.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	

GLvoid BuildFont(GLvoid);

int texture_num=3;
char texnames[3][32]={"art/grass1.bmp",
					  "art/StoneWall.bmp",
					  "art/clouds.bmp"};
GLuint texture[3];

extern TextureImage Fonts[1];
extern TextureImage *CBTexture;
extern TextureImage *Smoke;

int LoadGLTextures()
{
	LoadTGA(&Fonts[0],"art/font.tga", true);
	BuildFont();
	LoadTGA(CBTexture,"art/cannonball.tga");
	LoadTGA(Smoke,"art/smoke.tga");
	int Status=false;
	int counter;

	stbi_set_flip_vertically_on_load(false);
	for (counter=0;counter<texture_num;counter++)
	{
		int width, height, channels;
		void *imageData = stbi_load(texnames[counter], &width, &height, &channels, STBI_rgb);
		if (imageData) {
			Status = true;
			glGenTextures(1, &texture[counter]);
			glBindTexture(GL_TEXTURE_2D, texture[counter]);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			stbi_image_free(imageData);
		}
	}
	return Status;
}
