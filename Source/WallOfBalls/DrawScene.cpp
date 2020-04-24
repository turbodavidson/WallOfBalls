#include <windows.h>
#include <gl/gl.h>

#include "mesh.h"
#include "TextureImage.h"
#include "DataTypes.h"
#include "cannonball.h"

TextureImage *Water=new TextureImage;
TextureImage *Shotgun=new TextureImage;

extern vect2_t gunWobble;
extern bool gunMovement[2];
extern bool walking;

#include "bullet.h"
extern bullet *Bullet;

extern cannonball *CannonBall[40];

extern GLuint texture[3];
static GLuint Grass;
static mesh* myMesh=new mesh(true,25.0,false,0.0);
static mesh* myWater=new mesh(false,25.0,true,-.01);
static mesh* Clouds=new mesh(false,2.0,true,0.25);

//Wall Crap
static GLdouble wallcorners[]={-500.0,50.0,500.0,
						   500.0,50.0,500.0,
						   -500.0,-10.0,500.0,
						   500.0,-10.0,500.0,
						   //
						   -500.0,50.0,-500.0,
						   500.0,50.0,-500.0,
						   -500.0,-10.0,-500.0,
						   500.0,-10.0,-500.0};
static GLdouble walltexcoords[]={0.0,0.0,
								15.0,0.0,
								0.0,2.0,
								15.0,2.0,
								15.0,0.0,
								0.0,0.0,
								15.0,2.0,
								0.0,2.0};
static GLubyte wallindexes[]={1,0,2,
						1,2,3,
						6,4,5,
						7,6,5,
						2,0,4,
						6,2,4,
						5,1,7,
						1,3,7};
// End of Wall Crap

// TGA Crap
bool LoadTGA(TextureImage *texture, char *filename);
//

void CompileOriginalScene(void)
{
	myWater->Scale(1.2);
	Clouds->SetMeshPos(10.0);
	Clouds->PullPoint(25,25,80000);
	Clouds->PullPoint(20,25,80000);
	Clouds->PullPoint(20,25,80000);
	Clouds->PullPoint(25,20,80000);
	Clouds->PullPoint(25,20,80000);
	Clouds->Scale(3.0);
	for (int counter=0;counter<30;counter++)
	{
	Clouds->interpolateMesh();
	}
	LoadTGA(Shotgun,"art/Shotgun1.tga");
	LoadTGA(Water,"art/Water.tga");
	Grass=glGenLists(1);
	glNewList(Grass,GL_COMPILE);
	//
	// Draw Walls
	glPolygonMode(GL_FRONT,GL_LINE);
	glPolygonMode(GL_BACK,GL_FILL);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_DOUBLE,0,wallcorners);
	glTexCoordPointer(2,GL_DOUBLE,0,walltexcoords);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_BYTE,wallindexes);
	//
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	// Draw Layer 1 of Grass
	glBindTexture(GL_TEXTURE_2D,texture[0]);
	myMesh->renderMesh();
	// Water Stuff
	/*
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0.0,0.0);
	glVertex3d(-500.0,0.0,-500.0);
	glTexCoord2d(1.0,0.0);
	glVertex3d(500.0,0.0,-500.0);
	glTexCoord2d(0.0,1.0);
	glVertex3d(-500.0,0.0,500.0);
	glTexCoord2d(1.0,1.0);
	glVertex3d(500.0,0.0,500.0);
	glEnd();
	*/
	glEndList();
}

void CompileScene(void)
{
	LoadTGA(Shotgun,"art/Shotgun1.tga");
	LoadTGA(Water,"art/Water.tga");
	Grass=glGenLists(1);
	glNewList(Grass,GL_COMPILE);
	//
	// Draw Walls
	//glPolygonMode(GL_FRONT,GL_LINE);
	//glPolygonMode(GL_BACK,GL_FILL);
	//glCullFace(GL_BACK);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3,GL_DOUBLE,0,wallcorners);
	glTexCoordPointer(2,GL_DOUBLE,0,walltexcoords);
	glBindTexture(GL_TEXTURE_2D,texture[1]);
	glDrawElements(GL_TRIANGLES,24,GL_UNSIGNED_BYTE,wallindexes);
	//
	// Draw Layer 1 of Grass
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	glBindTexture(GL_TEXTURE_2D,texture[0]);
	myMesh->renderMesh();
	//
	// Water Stuff
	//glBindTexture(GL_TEXTURE_2D,Water->texID);
	//myWater->renderMesh();
	/*
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0.0,0.0);
	glVertex3d(-500.0,0.0,-500.0);
	glTexCoord2d(1.0,0.0);
	glVertex3d(500.0,0.0,-500.0);
	glTexCoord2d(0.0,1.0);
	glVertex3d(-500.0,0.0,500.0);
	glTexCoord2d(1.0,1.0);
	glVertex3d(500.0,0.0,500.0);
	glEnd();
	*/
	glEndList();
}

void DrawScene(void)
{
	
	////Drawing Clouds
	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

	// Gun Wobble Stuff
	if (walking)
	{
		if (gunMovement[0])
		{
			gunWobble.x-=float(0.00035);
			if (gunWobble.x<-0.02)
				gunMovement[0]=!gunMovement[0];
		}
		if (!gunMovement[0])
		{
			gunWobble.x+=float(0.00035);
			if (gunWobble.x>0.02)
				gunMovement[0]=!gunMovement[0];
		}
		if (gunMovement[1])
		{
			gunWobble.y+=float(0.0003);
			if (gunWobble.y>0.01)
				gunMovement[1]=!gunMovement[1];
		}
		if (!gunMovement[1])
		{
			gunWobble.y-=float(0.0003);
			if (gunWobble.y<-0.01)
				gunMovement[1]=!gunMovement[1];
		}
	}
	// Donw With That
	glCallList(Grass);

	glBindTexture(GL_TEXTURE_2D,texture[2]);
	Clouds->renderMesh();

	glBindTexture(GL_TEXTURE_2D,Water->texID);
	myWater->renderMesh();

	for (int counter=0;counter<40;counter++)
	CannonBall[counter]->render();

	Bullet->render();

	// Time to draw the shotgun
	//glDisable(GL_DEPTH_TEST);
	glLoadIdentity();
	glBindTexture(GL_TEXTURE_2D,Shotgun->texID);
	glBegin(GL_TRIANGLE_STRIP);

		glTexCoord2d(1.0,1.0);
		glVertex3d(gunWobble.x+0.15,gunWobble.y-0.1,-0.2);

		glTexCoord2d(0.0,1.0);
		glVertex3d(gunWobble.x,gunWobble.y-0.1,-0.2);

		glTexCoord2d(1.0,0.01);
		glVertex3d(gunWobble.x+0.15,gunWobble.y+0.05,-0.2);

		glTexCoord2d(0.0,0.01);
		glVertex3d(gunWobble.x,gunWobble.y+0.05,-0.2);

	glEnd();

}