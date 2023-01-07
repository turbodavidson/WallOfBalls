#include "cannonball.h"
#include "Camera.h"
#include "sound.h"
#include <math.h>

#include "Game.h"

//extern Camera *DefaultView;
extern SoundManager* Sounds;

float rnd(int);

#define DEG_TO_RAD 3.14159/180

GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);

cannonball::cannonball()
{
	position.x=0.0;
	position.y=0.0;
	position.z=-550.0;
	deployed=false;
	Timer=new timer_c();
	regenerate();
}

cannonball::~cannonball()
{
}

void cannonball::update()
{
	char x[20];
	char y[20];
	char z[20];

	Timer->update();
	double timepassed=Timer->getElapsedS();
	velocity.y=(initvelocityy+(-4.80665*timepassed));
	position.y=60.0+((0.5*(initvelocityy*velocity.y)*timepassed));
	itoa(int(position.x),x,10);
	itoa(int(position.y),y,10);
	itoa(int(position.z),z,10);

	//glPrint(0,15,1,x);
	//glPrint(0,45,1,y);
	//glPrint(0,75,1,z);

	position.z=-550.0+((0.5*(5.0*velocity.z)*timepassed));

	if (position.y<-5.0)	//Hit ground
	{
		regenerate();
		deploy();
	}
}

void cannonball::render()
{
	DriverGL11::Disable(GL_DEPTH_TEST);
	if (deployed) update();
	//glPrint(0,100,1,"Rendering...");
	//DriverGL11::BindTexture(GL_TEXTURE_2D,CBTexture->texID);
	DriverGL11::BindTexture(GL_TEXTURE_2D, G.cann.texId);

	DriverGL11::Begin(GL_TRIANGLE_STRIP);
	DriverGL11::TexCoord2d(0.0,1.0);
	double anglex=DEG_TO_RAD*G.DefaultView->getAngleY();
	double angley=DEG_TO_RAD*G.DefaultView->getAngleY();

	DriverGL11::Vertex3d	(position.x-(5.0*cos(anglex)),
				position.y+5.0,
				position.z-(5.0*sin(anglex)));
	DriverGL11::TexCoord2d(1.0,1.0);
	DriverGL11::Vertex3d	(position.x+(5.0*cos(anglex)),
				position.y+5.0,
				position.z+(5.0*sin(anglex)));
	DriverGL11::TexCoord2d(0.0,0.0);
	DriverGL11::Vertex3d	(position.x-(5.0*cos(anglex)),
				position.y-5.0,
				position.z-(5.0*sin(anglex)));
	DriverGL11::TexCoord2d(1.0,0.0);
	DriverGL11::Vertex3d	(position.x+(5.0*cos(anglex)),
				position.y-5.0,
				position.z+(5.0*sin(anglex)));
	DriverGL11::End();
	DriverGL11::Enable(GL_DEPTH_TEST);
}

void cannonball::deploy()
{
	//Sounds->play(TREB_SOUND);
	Timer->start();
	deployed=true;
	velocity.y=double(rnd(9)+5.0);
	initvelocityy=velocity.y;
	velocity.z=double(rnd(150)+10.0);
	velocity.x=0.0;
}

void cannonball::regenerate()
{
//	startfrom=int(rnd(4));

	position.x=double(rnd(1000)-500);
	position.y=60.0;
	position.z=-550.0;
	velocity.x=0.0;
	velocity.y=0.0;
	velocity.z=0.0;
	deployed=false;
}

bool cannonball::checkforcollision(double x,double y,double z)
{
	if (x>(position.x-15.0) && 
		x<(position.x+15.0) &&
		y>(position.y-15.0) &&
		y<(position.y+15.0) &&
		z>(position.z-15.0) &&
		z<(position.z+15.0))
		return true;
	return false;
}

vect3_t cannonball::getPosition()
{
	return position;
}