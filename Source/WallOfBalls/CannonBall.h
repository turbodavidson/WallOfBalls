#ifndef __CannonBall_h__
#define __CannonBall_h__

#include <windows.h>
#include <gl/gl.h>
#include <math.h>

#include "TextureImage.h"

#include "DataTypes.h"

extern TextureImage *CBTexture;
extern TextureImage *Smoke;

class cannonball
{
public:
	cannonball();
	~cannonball();

	void render();
	void deploy();
	bool checkforcollision(double,double,double);
	void regenerate();
	void update();

private:
	vect3_t position;
	vect3_t velocity;
	double initvelocityy;
	bool deployed;
	timer_c *Timer;
	bool dying;
	//int startfrom;
};

#endif
