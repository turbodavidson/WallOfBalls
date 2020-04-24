#include "mesh.h"

extern GLuint texture[];
const int interPolationLevel=1;

float rnd(int range);

mesh::mesh(bool interpolate,double texmult,bool scrolling,double amount)
{
	amt=amount;
	angle=0.0;
	if (interpolate==true)
	{
	for (int cx=0;cx<50;cx++)
		for (int cz=0;cz<50;cz++)
		{
			m_meshPoint[cx][cz].x=GLfloat(cx-25)*float(22.0);
			m_meshPoint[cx][cz].y=rnd(40)-20;
			if (m_meshPoint[cx][cz].y>6.0)
				m_meshPoint[cx][cz].y=6.0;
			if (m_meshPoint[cx][cz].y<-6.0)
				m_meshPoint[cx][cz].y=-6.0;
			m_meshPoint[cx][cz].z=GLfloat(cz-25)*float(22.0);
			m_meshPoint[cx][cz].tx=GLfloat((cx+1)/float(50))*texmult;
			m_meshPoint[cx][cz].ty=GLfloat((cz+1)/float(50))*texmult;
			m_meshPoint[cx][cz].nx=0.0;
			m_meshPoint[cx][cz].ny=1.0;
			m_meshPoint[cx][cz].nz=0.0;
		}
		interpolateMesh();
	}
	else
	{
		for (int cx=0;cx<50;cx++)
		for (int cz=0;cz<50;cz++)
		{
			m_meshPoint[cx][cz].x=GLfloat(cx-25)*float(25.0);
			m_meshPoint[cx][cz].y=0.0;
			m_meshPoint[cx][cz].z=GLfloat(cz-25)*float(25.0);
			m_meshPoint[cx][cz].tx=GLfloat((cx+1)/float(50))*10;
			m_meshPoint[cx][cz].ty=GLfloat((cz+1)/float(50))*10;
			m_meshPoint[cx][cz].nx=0.0;
			m_meshPoint[cx][cz].ny=1.0;
			m_meshPoint[cx][cz].nz=0.0;
		}
	}
}

mesh::~mesh()
{
}

void mesh::interpolateMesh()
{
	for (int times=0;times<interPolationLevel;times++)
	{
	for (int cx=1;cx<49;cx++)
		for (int cz=1;cz<49;cz++)
		{
			GLdouble tmp=0.0;
			tmp+=m_meshPoint[cx-1][cz-1].y;
			tmp+=m_meshPoint[cx][cz-1].y;
			tmp+=m_meshPoint[cx+1][cz-1].y;
			tmp+=m_meshPoint[cx-1][cz].y;
			// Add real 3 times, so it is true to form.
			tmp+=m_meshPoint[cx][cz].y;
			tmp+=m_meshPoint[cx][cz].y;
			tmp+=m_meshPoint[cx][cz].y;
			//
			tmp+=m_meshPoint[cx+1][cz].y;
			tmp+=m_meshPoint[cx-1][cz+1].y;
			tmp+=m_meshPoint[cx][cz+1].y;
			tmp+=m_meshPoint[cx+1][cz+1].y;
			tmp=tmp/11;
			m_meshPoint[cx][cz].y=tmp;
		}
	for (int cx=48;cx>0;cx--)
		for (int cz=48;cz>0;cz--)
		{
			GLdouble tmp=0.0;
			tmp+=m_meshPoint[cx-1][cz-1].y;
			tmp+=m_meshPoint[cx][cz-1].y;
			tmp+=m_meshPoint[cx+1][cz-1].y;
			tmp+=m_meshPoint[cx-1][cz].y;
			// Add real 3 times, so it is true to form.
			tmp+=m_meshPoint[cx][cz].y;
			tmp+=m_meshPoint[cx][cz].y;
			tmp+=m_meshPoint[cx][cz].y;
			//
			tmp+=m_meshPoint[cx+1][cz].y;
			tmp+=m_meshPoint[cx-1][cz+1].y;
			tmp+=m_meshPoint[cx][cz+1].y;
			tmp+=m_meshPoint[cx+1][cz+1].y;
			tmp=tmp/11;
			m_meshPoint[cx][cz].y=tmp;
		}
	}
}

void mesh::setCentre(GLfloat y)
{
	m_meshPoint[25][25].y=y;
	interpolateMesh();
}

void mesh::renderMesh()
{
	angle+=amt;
	glRotated(angle,0.0,1.0,0.0);
	if (angle>360.0)
		angle=angle-360.0;
	if (angle<-360.0)
		angle+=360.0;
	for (int cx=1;cx<48;cx++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for (int cz=1;cz<49;cz++)
		{
			glTexCoord2d(m_meshPoint[cx][cz].tx,m_meshPoint[cx][cz].ty);
			glVertex3d( m_meshPoint[cx][cz].x,
						m_meshPoint[cx][cz].y,
						m_meshPoint[cx][cz].z);
			glTexCoord2d(m_meshPoint[cx+1][cz].tx,m_meshPoint[cx+1][cz].ty);
			glVertex3d( m_meshPoint[cx+1][cz].x,
						m_meshPoint[cx+1][cz].y,
						m_meshPoint[cx+1][cz].z);
		}
		glEnd();
	}
	glRotated(-angle,0.0,1.0,0.0);
}

void mesh::SetMeshPos(double y)
{
	for (int cx=0;cx<50;cx++)
		for (int cz=0;cz<50;cz++)
		{
		
			m_meshPoint[cx][cz].y+=y;
		}
}

void mesh::PullPoint(int x,int y,double height)
{
	m_meshPoint[x][y].y=height;
}

void mesh::Scale(double howmuch)
{
	for (int cx=0;cx<50;cx++)
		for (int cz=0;cz<50;cz++)
		{
		
			m_meshPoint[cx][cz].x*=howmuch;
			m_meshPoint[cx][cz].z*=howmuch;
		}
}