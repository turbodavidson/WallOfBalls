#include "Game.h"

#include "GlobalEnum.h"
#include "sound.h"
#include "Font.h"

Game G;

//SysVar
GLuint CURRENT_SCREENMODE=3;
GLuint CURRENT_SCREENBPP=1;
GLuint MAX_SCREENMODES=4;
GLuint MAX_SCREENBPP=2;
GLuint app_ScreenModes[4][2]=	{
								640,480,
								800,600,
								1024,768,
								2560,1440
								};
GLuint app_ScreenBPP[2] = {16,32};


//Function Declarations
/////////////////////////////////////////////////////////////////////////////////////////

void seedrnd(void);
void LoadSounds(void);
/////////////////////////////////////////////////////////////////////////////////////////

//Resources
SoundManager* Sounds;

void Game::Init(void)
{
	CreditTime = new timer_c();
	app_StillRunning = true; 
	timeofday = 1;
	i = 780;
	done_with_credits = game_started = game_over = won = walking = false;

	timeofdaycolors[0][0]= 1.0;timeofdaycolors[0][1]= 1.0;timeofdaycolors[0][2]= 0.85;timeofdaycolors[0][3]= 1.0;
	timeofdaycolors[1][0]= 1.0;timeofdaycolors[1][1]= 1.0;timeofdaycolors[1][2]= 1.0;timeofdaycolors[1][3]= 1.0;
	timeofdaycolors[2][0]= 0.5;timeofdaycolors[2][1]= 0.5;timeofdaycolors[2][2]= 1.0;timeofdaycolors[2][3]= 1.0;

	//Window
	S.WindowWidth = app_ScreenModes[CURRENT_SCREENMODE][0];
	S.WindowHeight = app_ScreenModes[CURRENT_SCREENMODE][1];
	S.WindowBitsPP = app_ScreenBPP[CURRENT_SCREENBPP];
	S.WindowFullScreen = true;
	S.Init();

	// Do key bindings
	S.Bind(VK_ESCAPE,EXIT);
	S.Bind(VK_F1,SWITCH_SCREENMODE);
	S.Bind(VK_F2,SWITCH_SCREENBPP);
	S.Bind(VK_F3,SWITCH_FULLSCREEN);
	S.Bind('W',MOVE_FOREWARD);
	S.Bind('S',MOVE_BACKWARD);
	S.Bind('A',MOVE_LEFT);
	S.Bind('D',MOVE_RIGHT);
	S.Bind('Q',LOOK_LEFT);
	S.Bind('E',LOOK_RIGHT);
	S.Bind('R',LOOK_UP);
	S.Bind('F',LOOK_DOWN);
	S.Bind(VK_F4,NEW_GAME);
	S.Bind(VK_F5,KEY_FIREGUN);
	S.Bind(VK_SPACE,JUMP);
	S.Bind (VK_F8,SAVE_SCREEN);
	S.Bind(VK_LEFT,KEY_DECREASETRACK);
	S.Bind(VK_RIGHT,KEY_INCREASETRACK);
	S.Bind(VK_F9,SWITCH_TOD);

	// GL
	DriverGL11::Enable(GL_TEXTURE_2D);
	DriverGL11::Enable(GL_DEPTH_TEST);
	DriverGL11::ShadeModel(GL_FLAT);
	DriverGL11::ClearDepth(1.0);
	DriverGL11::ClearColor(0.0,0.0,0.0,1.0f);
	DriverGL11::Hint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	DriverGL11::Enable(GL_BLEND);
	DriverGL11::BlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	DriverGL11::DepthFunc(GL_LEQUAL);

	S.GetGL11Driver()->ReSizeGLScene(S.WindowWidth, S.WindowHeight);

	cann.texture = S.GetResource(10);
	tmpLoadTexture(cann);

	font.texture = S.GetResource(30);
	tmpLoadTexture(font);

	smok.texture = S.GetResource(60);
	tmpLoadTexture(smok);

	gras.texture = S.GetResource(42);
	tmpLoadTexture(gras, true);

	wall.texture = S.GetResource(90);
	tmpLoadTexture(wall, true);

	clds.texture = S.GetResource(80);
	tmpLoadTexture(clds, true);

	gunn.texture = S.GetResource(50);
	tmpLoadTexture(gunn);

	watr.texture = S.GetResource(70);
	tmpLoadTexture(watr);

	BuildFont();

	for (int i=0;i<40;i++) {
		cannonBallRender[i].distance = 0.0f;
		cannonBallRender[i].cb =new cannonball();
	}

	//for (int counter=0;counter<40;counter++)
	//	CannonBall[counter]=new cannonball();
	Bullet=new bullet();

	// Sound Stuff
	Sounds=new SoundManager(S.tmpGetWindowHandle());
	LoadSounds();
	Sounds->play(STORY_NARRATION);
	S.AddCommand(KEY_INTROMUSIC);
	credit_stage=false;

	CompileOriginalScene();
	DefaultView = new Camera();
	gunMovement[0]=true;
	gunMovement[1]=true;

	gunWobble.x=0.0;
	gunWobble.y=0.0;

	seedrnd();
}

void Game::tmpLoadTexture(Texture &tex, bool mipmap)
{
	DriverGL11::GenTextures(1, &tex.texId);
	DriverGL11::BindTexture(GL_TEXTURE_2D, tex.texId);
	DriverGL11::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	DriverGL11::TexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	DriverGL11::TexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ((TargaImage*)tex.texture)->GetWidth(), ((TargaImage*)tex.texture)->GetHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.texture->Data());
	if (mipmap)
	{
		gluBuild2DMipmaps(GL_TEXTURE_2D, 3, ((TargaImage*)tex.texture)->GetWidth(), ((TargaImage*)tex.texture)->GetHeight(), GL_RGBA, GL_UNSIGNED_BYTE, tex.texture->Data());
	}
}

void Game::Free(void)
{
	S.Free();
	delete Sounds;
	delete Bullet;
	delete DefaultView;
	//for (int counter=0;counter<40;counter++)
	//	delete CannonBall[counter];
	for (int i=0;i<40;i++) {
		delete cannonBallRender[i].cb;
	}
	delete Clouds;
	delete myWater;
	delete myMesh;
}

void Game::CompileOriginalScene(void)
{
	myWater->Scale(1.2);
	Clouds->SetMeshPos(10.0);
	Clouds->PullPoint(25,25,80000);
	Clouds->PullPoint(20,25,80000);
	Clouds->PullPoint(20,25,80000);
	Clouds->PullPoint(25,20,80000);
	Clouds->PullPoint(25,20,80000);
	Clouds->Scale(3.0);
	for (int counter=0;counter<30;counter++)
	{
	Clouds->interpolateMesh();
	}
	Grass=DriverGL11::GenLists(1);
	DriverGL11::NewList(Grass,GL_COMPILE);
	//
	// Draw Walls
	DriverGL11::PolygonMode(GL_FRONT,GL_LINE);
	DriverGL11::PolygonMode(GL_BACK,GL_FILL);
	DriverGL11::EnableClientState(GL_TEXTURE_COORD_ARRAY);
	DriverGL11::EnableClientState(GL_VERTEX_ARRAY);
	DriverGL11::VertexPointer(3,GL_DOUBLE,0,wallcorners);
	DriverGL11::TexCoordPointer(2,GL_DOUBLE,0,walltexcoords);
	//DriverGL11::BindTexture(GL_TEXTURE_2D,texture[1]);
	DriverGL11::BindTexture(GL_TEXTURE_2D,wall.texId);
	DriverGL11::DrawElements(GL_TRIANGLES,24,GL_UNSIGNED_BYTE,wallindexes);
	//
	DriverGL11::PolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	// Draw Layer 1 of Grass
	//DriverGL11::BindTexture(GL_TEXTURE_2D,texture[0]);
	DriverGL11::BindTexture(GL_TEXTURE_2D, gras.texId);
	myMesh->renderMesh();
	// Water Stuff
	/*
	glBegin(GL_TRIANGLE_STRIP);
	glTexCoord2d(0.0,0.0);
	glVertex3d(-500.0,0.0,-500.0);
	glTexCoord2d(1.0,0.0);
	glVertex3d(500.0,0.0,-500.0);
	glTexCoord2d(0.0,1.0);
	glVertex3d(-500.0,0.0,500.0);
	glTexCoord2d(1.0,1.0);
	glVertex3d(500.0,0.0,500.0);
	glEnd();
	*/
	DriverGL11::EndList();
}

void Game::DrawScene(void)
{
	// Grass
	DriverGL11::PolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	DriverGL11::Enable(GL_DEPTH_TEST);
	DriverGL11::CallList(Grass);

	// Clouds
	DriverGL11::BindTexture(GL_TEXTURE_2D, clds.texId);
	Clouds->renderMesh();

	// Water
	DriverGL11::BindTexture(GL_TEXTURE_2D, watr.texId);
	myWater->renderMesh();

	// Cannon Balls
	double playerX = DefaultView->getX();
	double playerY = DefaultView->getY();
	double playerZ = DefaultView->getZ();

	for(int i=0; i<40; i++) {
		vect3_t ballpos = cannonBallRender[i].cb->getPosition();
		float x = playerX - ballpos.x;
		float y = playerY - ballpos.y;
		float z = playerZ - ballpos.z;
		cannonBallRender[i].distance = sqrt((x * x) + (y * y) + (z * z));
	}
	bool swapped;
	do {
		swapped = false;
		for(int i=0; i<39; i++) {
			if(cannonBallRender[i + 1].distance > cannonBallRender[i].distance) {
				CannonBallRender tmp = cannonBallRender[i];
				cannonBallRender[i] = cannonBallRender[i + 1];
				cannonBallRender[i + 1] = tmp;
				swapped = true;
			}
		}
	} while(swapped);

	DriverGL11::DepthMask(GL_FALSE);
	for (int counter = 0;counter < 40;counter++) {
		cannonBallRender[counter].cb->render();
	}
	DriverGL11::DepthMask(GL_TRUE);

	// Bullet
	Bullet->render();

	// Gun
	if (walking)
	{
		if (gunMovement[0])
		{
			gunWobble.x-=float(0.00035);
			if (gunWobble.x<-0.02)
				gunMovement[0]=!gunMovement[0];
		}
		if (!gunMovement[0])
		{
			gunWobble.x+=float(0.00035);
			if (gunWobble.x>0.02)
				gunMovement[0]=!gunMovement[0];
		}
		if (gunMovement[1])
		{
			gunWobble.y+=float(0.0003);
			if (gunWobble.y>0.01)
				gunMovement[1]=!gunMovement[1];
		}
		if (!gunMovement[1])
		{
			gunWobble.y-=float(0.0003);
			if (gunWobble.y<-0.01)
				gunMovement[1]=!gunMovement[1];
		}
	}

	DriverGL11::Disable(GL_DEPTH_TEST);
	DriverGL11::LoadIdentity();
	DriverGL11::BindTexture(GL_TEXTURE_2D, gunn.texId);
	DriverGL11::Begin(GL_TRIANGLE_STRIP);

		DriverGL11::TexCoord2d(1.0,1.0);
		DriverGL11::Vertex3d(gunWobble.x+0.15,gunWobble.y-0.1,-0.2);

		DriverGL11::TexCoord2d(0.0,1.0);
		DriverGL11::Vertex3d(gunWobble.x,gunWobble.y-0.1,-0.2);

		DriverGL11::TexCoord2d(1.0,0.01);
		DriverGL11::Vertex3d(gunWobble.x+0.15,gunWobble.y+0.05,-0.2);

		DriverGL11::TexCoord2d(0.0,0.01);
		DriverGL11::Vertex3d(gunWobble.x,gunWobble.y+0.05,-0.2);

	DriverGL11::End();
}