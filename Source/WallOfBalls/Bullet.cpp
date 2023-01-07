#include "Bullet.h"

#include "Camera.h"
#include "cannonball.h"
#include <math.h>

#include "Game.h"

//extern Camera *DefaultView;
//extern cannonball *CannonBall[40];

float rnd(int);

#define DEG_TO_RAD 3.14159/180.0

GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);

bullet::bullet()
{
	deployed=false;
	Timer=new timer_c();
}

bullet::~bullet()
{
}

void bullet::update()
{
	char x[20];
	char y[20];
	char z[20];

	Timer->update();
	double timepassed=Timer->getElapsedS();

	position.x=initpos.x+(velocity.x*timepassed)*cos(G.DefaultView->getAngleXRAD());
	position.y=initpos.y+velocity.y*timepassed;
	position.z=initpos.z+(velocity.z*timepassed)*cos(G.DefaultView->getAngleXRAD());

	itoa(int(position.x),x,10);
	itoa(int(position.y),y,10);
	itoa(int(position.z),z,10);

	//glPrint(0,15,1,x);
	//glPrint(0,45,1,y);
	//glPrint(0,75,1,z);
	
	if (position.z>515.0 || position.z<-515.0)
		deployed=false;
	if (position.y>300.0 || position.y<0.0)
		deployed=false;
	if (position.x>515.0 || position.x<-515.0)
		deployed=false;

	//for (int counter=0;counter<40;counter++)
	//	if (G.CannonBall[counter]->checkforcollision(position.x,position.y,position.z))
	//	{
	//		deployed=false;
	//		G.CannonBall[counter]->regenerate();
	//		G.CannonBall[counter]->deploy();
	//		G.DefaultView->incscore();
	//	}

	for (int counter=0;counter<40;counter++)
		if (G.cannonBallRender[counter].cb->checkforcollision(position.x,position.y,position.z))
		{
			deployed=false;
			G.cannonBallRender[counter].cb->regenerate();
			G.cannonBallRender[counter].cb->deploy();
			G.DefaultView->incscore();
		}

}

void bullet::render()
{
	if (deployed) 
	{update();
	//glPrint(0,100,1,"Rendering...");
	//DriverGL11::BindTexture(GL_TEXTURE_2D, CBTexture->texID);
	DriverGL11::BindTexture(GL_TEXTURE_2D, G.cann.texId);

	DriverGL11::Begin(GL_TRIANGLE_STRIP);
	double anglex=DEG_TO_RAD*G.DefaultView->getAngleY();
	double angley=DEG_TO_RAD*G.DefaultView->getAngleY();
	//double anglex=0.0;
	//double angley=0.0;

	DriverGL11::TexCoord2d(0.0,1.0);
	DriverGL11::Vertex3d	(position.x-(0.5*cos(anglex)),
				position.y-0.5,
				position.z-(0.5*sin(anglex)));
	DriverGL11::TexCoord2d(2.0,1.0);
	DriverGL11::Vertex3d	(position.x+(0.5*cos(anglex)),
				position.y-0.5,
				position.z+(0.5*sin(anglex)));
	DriverGL11::TexCoord2d(0.0,0.0);
	DriverGL11::Vertex3d	(position.x-(0.5*cos(anglex)),
				position.y+0.5,
				position.z-(0.5*sin(anglex)));
	DriverGL11::TexCoord2d(2.0,0.0);
	DriverGL11::Vertex3d	(position.x+(0.5*cos(anglex)),
				position.y+0.5,
				position.z+(0.5*sin(anglex)));
	DriverGL11::End();
	}
}

void bullet::deploy()
{
	if (!deployed)
	{
	Timer->start();
	deployed=true;

	initpos.x=-G.DefaultView->getX();
	initpos.y=-G.DefaultView->getY();
	initpos.z=-G.DefaultView->getZ();

	velocity.x=1500.0*sin(G.DefaultView->getAngleYRAD());
	velocity.z=-1500.0*cos(G.DefaultView->getAngleYRAD());
	velocity.y=-1500.0*sin(G.DefaultView->getAngleXRAD());;
	}
}
