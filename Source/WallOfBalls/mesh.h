#include <windows.h>
#include <gl/gl.h>

struct Points
{
	GLdouble x;
	GLdouble y;
	GLdouble z;
	GLdouble tx;
	GLdouble ty;
	GLdouble nx;
	GLdouble ny;
	GLdouble nz;
};

class mesh
{
public:
	mesh(bool,double,bool,double);
	~mesh(void);
	void renderMesh(void);
	void interpolateMesh(void);
	void setCentre(GLfloat);
	void SetMeshPos(double);
	void PullPoint(int,int,double);
	void Scale(double);
private:
	Points m_meshPoint[50][50];
	double angle;
	double amt;
};