#ifndef __Particles_h__
#define __Particles_h__

#include <windows.h>
#include <gl/gl.h>

#include "DataTypes.h"

class particles
{
public:
	particles(); ///howmany,vertvelocity,x,y,z
	~particles();

	void start(int,double,double,double);
	bool update();
	void render();
private:
	vect3_t pos;
	vect3_t *particle;
	vect3_t *velocity;
	int howmany;
	double *initvelocityy;
	double inittx;
	double initty;
	double inittz;
	timer_c *Timer;
	bool *active;
	int numofactive;
};

#endif