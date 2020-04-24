#ifndef __DataTypes_h__
#define __DataTypes_h__

#include <windows.h>
#include <gl/gl.h>

typedef struct
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
} vect3_t;

typedef struct
{
	GLdouble x;
	GLdouble y;
} vect2_t;

// timer_c class
class timer_c
{
public:
	timer_c();
	~timer_c();

	void start(void);
	void update(void);

	double getElapsedS(void);
	int getElapsedMS(void);
private:
	int m_start;
	double m_elapsed_s;
	int m_elapsed_ms;
};

// End of timer_c class

#endif