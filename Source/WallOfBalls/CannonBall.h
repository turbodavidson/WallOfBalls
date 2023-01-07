#ifndef __CannonBall_h__
#define __CannonBall_h__

#include <Windows.h>
#include <gl/gl.h>
#include <math.h>

#include "DataTypes.h"

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
	vect3_t getPosition();

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
