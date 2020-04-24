#ifndef __DCamera_h__
#define __DCamera_h__

#include "DataTypes.h"

class Camera
{
public:
	Camera();
	~Camera();
	void Advance(double);
	void Strafe(double);
	void Levitate(bool);
	void LookSideWays(double);
	void LookUp(double);
	void LookAt();
	void Jump(void);
	//
	double getAngleX(void);
	double getAngleY(void);
	double getAngleZ(void);
	//
	double getAngleXRAD(void);
	double getAngleYRAD(void);
	double getAngleZRAD(void);
	//
	double getX(void);
	double getY(void);
	double getZ(void);
	//
	void Clear(void);
	void incscore(void);
	void drawscore(void);
private:
	int score;
	double Deg_to_Rad;
	double Pos[3];
	double Angle[3];
	timer_c *Timer;
	bool jumping;
	vect3_t velocity;
	double gravity;
	int m_jumpStartTime;
};

#endif
