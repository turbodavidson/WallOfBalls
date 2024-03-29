#ifndef __Bullet_h__
#define __Bullet_h__

#include <windows.h>
#include <gl/gl.h>
#include <math.h>

#include "DriverGL11.h"
#include "DataTypes.h"

class bullet
{
public:
	bullet();
	~bullet();

	void render();
	void deploy();
	void update();

private:
	vect3_t position;
	vect3_t initpos;
	vect3_t velocity;
	bool deployed;
	timer_c *Timer;
};

#endif