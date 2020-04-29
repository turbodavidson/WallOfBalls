//// LoadBMP.h
//// (C)2002 MissileWar3D Team
//
//#ifndef __LoadBMP_h__
//#define __LoadBMP_h__
//
//#include <windows.h>
//#include <gl/gl.h>
//#include <gl/glu.h>
//#include "gl/glaux.h"
//#include <stdio.h>
//
//AUX_RGBImageRec *LoadBMP(char *Filename)
//{
//	FILE *File=NULL;
//	if (!Filename)
//	{
//		return NULL;
//	}
//	File=fopen(Filename,"r");
//	if (File)
//	{
//		fclose(File);
//		return auxDIBImageLoad(Filename);
//	}
//	return NULL;
//}
//
//int LoadTextureBMP(GLuint *texture,char *filename)
//{
//	int Status=false;
//	AUX_RGBImageRec *TextureImage[1];
//	memset(TextureImage,0,sizeof(void *)*1);
//	if (TextureImage[0]=LoadBMP(filename))
//	{
//		Status=true;
//		glGenTextures(1, texture);
//		glBindTexture(GL_TEXTURE_2D,*texture);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
//		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
//		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, TextureImage[0]->sizeX, TextureImage[0]->sizeY, GL_RGB, GL_UNSIGNED_BYTE, TextureImage[0]->data);
//	}
//	if (TextureImage[0])
//	{
//		if (TextureImage[0]->data)
//		{
//			free(TextureImage[0]->data);
//		}
//		free(TextureImage[0]);
//	}
//	return Status;
//}
//
//#endif
