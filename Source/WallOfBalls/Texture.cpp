#include <windows.h>
#include <gl/gl.h>
#include <gl/glu.h>
#include "gl/glaux.h"
#include <stdio.h>

#include "TextureImage.h"

bool LoadTGA(TextureImage *, char *);
GLvoid BuildFont(GLvoid);

int texture_num=3;
char texnames[3][32]={"art/grass1.bmp",
					  "art/StoneWall.bmp",
					  "art/clouds.bmp"};
GLuint texture[3];

AUX_RGBImageRec *LoadBMP(char *Filename);

extern TextureImage Fonts[1];
extern TextureImage *CBTexture;
extern TextureImage *Smoke;



int LoadGLTextures()
{
	LoadTGA(&Fonts[0],"art/font.tga");
	BuildFont();
	LoadTGA(CBTexture,"art/cannonball.tga");
	LoadTGA(Smoke,"art/smoke.tga");
	int Status=false;
	AUX_RGBImageRec *TextureImage[3];
	memset(TextureImage,0,sizeof(void *)*1);
	int counter;
	for (counter=0;counter<texture_num;counter++)
	{
		if (TextureImage[counter]=LoadBMP(texnames[counter]))
		{
			Status=true;
			glGenTextures(1, &texture[counter]);
			glBindTexture(GL_TEXTURE_2D,texture[counter]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
			gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[counter]->sizeX, TextureImage[counter]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[counter]->data);
		}
		if (TextureImage[counter])
		{
			if (TextureImage[counter]->data)
			{
				free(TextureImage[counter]->data);
			}
			free(TextureImage[counter]);
		}
	}
	return Status;
}

AUX_RGBImageRec *LoadBMP(char *Filename)
{
	FILE *File=NULL;
	if (!Filename)
	{
		return NULL;
	}
	File=fopen(Filename,"r");
	if (File)
	{
		fclose(File);
		return auxDIBImageLoad(Filename);
	}
	return NULL;
}