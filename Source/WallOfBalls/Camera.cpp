#include <windows.h>
#include <gl/gl.h>
#include <math.h>
#include "Camera.h"
#include "DataTypes.h"
#include "sound.h"
#include "cannonball.h"

#include "Game.h"

//extern cannonball *CannonBall[20];

extern SoundManager* Sounds;

//extern bool game_over;
//extern bool won;

//GLvoid glPrint(GLint x, GLint y, int set, const char *fmt, ...);

Camera::Camera()
{
	Deg_to_Rad=3.14159/180;
	Pos[0]=Pos[2]=0.0;
	Pos[1]=-6.5;
	Angle[0]=Angle[1]=Angle[2]=0.0;
	Timer=new timer_c();
	jumping=false;
	gravity=-9.80665;
	score=0;
	prevX = 512;
	prevY = 384;
}

Camera::~Camera()
{
}

void Camera::Advance(double amount)
{
	double TempAngle=Angle[1]*Deg_to_Rad;
		Pos[0]=Pos[0]-(sin(TempAngle)*amount);
		Pos[2]=Pos[2]+(cos(TempAngle)*amount);
}

void Camera::Strafe(double amount)
{
	double TempAngle=Angle[1]*Deg_to_Rad;

		Pos[0]=Pos[0]+(cos(TempAngle)*amount);
		Pos[2]=Pos[2]+(sin(TempAngle)*amount);
}

void Camera::Levitate(bool up)
{
	if(up)
	{
		Pos[1]-=1.0;
	}
	else
	{
		Pos[1]+=1.0;
	}
}

void Camera::LookSideWays(double amount)
{
		Angle[1]=Angle[1]+amount;
		if (Angle[1]<0.0)
			Angle[1]=359.0;
}

void Camera::LookUp(double amount)
{
		Angle[0]=Angle[0]+amount;
		if (Angle[0]<-90.0)
			Angle[0]=-90.0;
		if (Angle[0]>90.0)
			Angle[0]=90.0;
}

bool Camera::Look(int x, int y)
{
	bool clampCursor = false;
	LookSideWays(-(double(prevX-(x)))/8.0);
	prevX = x;
	LookUp(-(double(prevY-(y)))/8.0);
	prevY = y;

	if (x < 312 || x > 712)
	{
		clampCursor = true;
		prevX = 512;
		prevY = 384;
	}
	if (y < 184 || y > 584)
	{
		clampCursor = true;
		prevX = 512;
		prevY = 384;
	}
	return clampCursor;
}

void Camera::LookAt()
{
	Timer->update();

	for (int counter=0;counter<40;counter++)
	if (G.cannonBallRender[counter].cb->checkforcollision(-Pos[0],-Pos[1],-Pos[2]))
	{
		G.game_over=true;
	}

	if (jumping)
	{
		double timepassed=Timer->getElapsedS();
		velocity.y=(10.0+(gravity*timepassed));
		Pos[1]=-6.5-((0.5*(10.0*velocity.y)*timepassed));
	}

	if (Pos[1]>-6.5)
	{
		jumping=false;
		Sounds->play(JUMPLAND_SOUND);
		Pos[1]=-6.5;
	}
	
	if (Pos[0]>499.0)
			Pos[0]=499.0;
	if (Pos[0]<-499.0)
			Pos[0]=-499.0;

	if (Pos[2]>499.0)
			Pos[2]=499.0;
	if (Pos[2]<-499.0)
			Pos[2]=-499.0;
	
	DriverGL11::Rotated(Angle[0],1.0,0.0,0.0);
	DriverGL11::Rotated(Angle[1],0.0,1.0,0.0);
	DriverGL11::Rotated(Angle[2],0.0,0.0,1.0);
	DriverGL11::Translated(Pos[0],Pos[1],Pos[2]);
}

void Camera::Jump(void)
{
	if (!jumping)
	{
		Sounds->play(JUMP_SOUND);
		Timer->start();
		velocity.y=10.0;
		jumping=true;
	}
}

double Camera::getAngleX(void)
{
	return Angle[0];
}

double Camera::getAngleY(void)
{
	return Angle[1];
}

double Camera::getAngleZ(void)
{
	return Angle[2];
}
//

double Camera::getAngleXRAD(void)
{
	return (Angle[0]*Deg_to_Rad);
}

double Camera::getAngleYRAD(void)
{
	return (Angle[1]*Deg_to_Rad);
}

double Camera::getAngleZRAD(void)
{
	return (Angle[2]*Deg_to_Rad);
}
//

double Camera::getX(void)
{
	return Pos[0];
}

double Camera::getY(void)
{
	return Pos[1];
}

double Camera::getZ(void)
{
	return Pos[2];
}


void Camera::Clear(void)
{
	Pos[0]=Pos[2]=0.0;
	Pos[1]=-6.5;
	Angle[0]=Angle[1]=Angle[2]=0.0;
}

void Camera::incscore()
{
	score+=1;
	if (score>=101)
	{
		G.won=true;
		G.game_over=true;
	}
}

void Camera::drawscore()
{
	char myscore[16];
	itoa(score,myscore,10);

	DriverGL11::Print(0, 10, 1, "Score: ");
	DriverGL11::Print(90, 10, 1, myscore);
	//glPrint(0,10,1,"Score: ");
	//glPrint(90,10,1,myscore);
}