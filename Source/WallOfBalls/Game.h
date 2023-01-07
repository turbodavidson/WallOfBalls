#ifndef __GAME_H
#define __GAME_H

#include "System.h"

#include "DataTypes.h"
#include "cannonball.h"
#include "Bullet.h"
#include "Camera.h"
#include "mesh.h"
#include "TargaImage.h"

bool Dispatch(int);

//Wall Crap
static GLdouble wallcorners[]={-500.0,50.0,500.0,
						   500.0,50.0,500.0,
						   -500.0,-10.0,500.0,
						   500.0,-10.0,500.0,
						   //
						   -500.0,50.0,-500.0,
						   500.0,50.0,-500.0,
						   -500.0,-10.0,-500.0,
						   500.0,-10.0,-500.0};
static GLdouble walltexcoords[]={0.0,0.0,
								15.0,0.0,
								0.0,2.0,
								15.0,2.0,
								15.0,0.0,
								0.0,0.0,
								15.0,2.0,
								0.0,2.0};
static GLubyte wallindexes[]={1,0,2,
						1,2,3,
						6,4,5,
						7,6,5,
						2,0,4,
						6,2,4,
						5,1,7,
						1,3,7};

static mesh* myMesh = new mesh(true,25.0,false,0.0);
static mesh* myWater = new mesh(false,25.0,true,-.01);
static mesh* Clouds = new mesh(false,2.0,true,0.25);

struct Texture
{
	unsigned int texId;
	ResourceType* texture;
};

typedef struct CannonBallRender {
	cannonball *cb;
	float distance;
} CannonBallRender;

class Game
{
public:
	// Logics
	bool app_StillRunning;
	bool walking;
	bool game_started;
	bool game_over;
	bool credit_stage;
	timer_c *CreditTime;
	int timeofday;
	double timeofdaycolors[3][4];
	bool won;
	bool credtimerstarted;
	int i;
	bool done_with_credits;

	// Game Objects
	Camera *DefaultView;
	//cannonball *CannonBall[40];
	CannonBallRender cannonBallRender[40];
	bullet *Bullet;
	vect2_t gunWobble;
	bool gunMovement[2];
	GLuint Grass;

	Texture font;
	Texture cann;
	Texture smok;
	Texture gras;
	Texture wall;
	Texture clds;
	Texture gunn;
	Texture watr;
public:
	Game() { };
	~Game() { };

private:
	void tmpLoadTexture(Texture &tex, bool mipmap = false);

public:
	void CompileOriginalScene(void);
	void CompileScene(void);
	void DrawScene(void);

public:
	void Init(void);
	void Exec(void);
	void Free(void);
};

extern Game G;

#endif