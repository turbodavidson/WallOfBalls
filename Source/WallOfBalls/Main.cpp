// Main.cpp

#define APP_NAME "Wall of Balls 1.0"

#include <windows.h>
#include <gl/gl.h>

#include "GlobalEnum.h"
#include "DataTypes.h"
#include "sound.h"
#include "Camera.h"
#include "Font.h"
#include "TextureImage.h"
#include "cannonball.h"
#include "Bullet.h"

//Function Declarations
/////////////////////////////////////////////////////////////////////////////////////////
bool Dispatch(int);
bool AddCommand(int Message);
bool Bind(GLuint Key,GLuint MessageToBind);
void LoadSounds(void);
int LoadGLTextures();
void CompileOriginalScene(void);
void DrawScene(void);
bool LoadTGA(TextureImage *, char *);
void seedrnd(void);
void LoadSounds();
/////////////////////////////////////////////////////////////////////////////////////////

//Global declarations
/////////////////////////////////////////////////////////////////////////////////////////
HGLRC	    hRC=NULL;
HDC			hDC=NULL;
HWND		hWnd=NULL;
HINSTANCE	hInstance;

//Fonts
TextureImage Fonts[1];
GLuint base;
//

bool app_StillRunning=true;
bool app_Active=true;
bool app_Fullscreen=true;

GLuint KeyMessage[256];
bool Keys[256];

int timeofday;
double timeofdaycolors[3][4]={1.0,1.0,0.85,1.0,
							  1.0,1.0,1.0,1.0,
							  0.5,0.5,1.0,1.0};

GLuint MAX_SCREENMODES=4;
GLuint MAX_SCREENBPP=2;

GLuint app_ScreenModes[4][2]=	{
								640,480,
								800,600,
								1024,768,
								1680,1050
								};

GLuint app_ScreenBPP[2]={16,32};


GLint cmdBuffer[32];



Camera *DefaultView;
bool game_started;
bool game_over;
bool credit_stage;
bool done_with_credits;

GLuint CURRENT_SCREENMODE=2;
GLuint CURRENT_SCREENBPP=1;

vect2_t gunWobble;
bool gunMovement[2];
bool walking;
bool won;
timer_c *CreditTime=new timer_c();
bool credtimerstarted;
//SoundInstansiations
/////////////////////////////////////////////////////////////////////////////////////////
SoundManager* Sounds;


/////////////////////////////////////////////////////////////////////////////////////////
TextureImage *CBTexture=new TextureImage;
TextureImage *Smoke=new TextureImage;

cannonball *CannonBall[40];

bullet *Bullet;
int i = 780;

int mouseprevx;
int mouseprevy;
/////////////////////////////////////////////////////////////////////////////////////////

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	
	timeofday=1;
	for (int KeySetup=0;KeySetup<256;KeySetup++)KeyMessage[KeySetup]=KeySetup;
	// Do key bindings
	Bind(VK_ESCAPE,EXIT);
	Bind(VK_F1,SWITCH_SCREENMODE);
	Bind(VK_F2,SWITCH_SCREENBPP);
	Bind(VK_F3,SWITCH_FULLSCREEN);
	//
	Bind('W',MOVE_FOREWARD);
	Bind('S',MOVE_BACKWARD);
	Bind('A',MOVE_LEFT);
	Bind('D',MOVE_RIGHT);
	//
	Bind('Q',LOOK_LEFT);
	Bind('E',LOOK_RIGHT);
	Bind('R',LOOK_UP);
	Bind('F',LOOK_DOWN);
	//
	Bind(VK_F4,NEW_GAME);
	Bind(VK_F5,KEY_FIREGUN);
	Bind(VK_SPACE,JUMP);
	//
	Bind (VK_F8,SAVE_SCREEN);
	
	Bind(VK_LEFT,KEY_DECREASETRACK);
	Bind(VK_RIGHT,KEY_INCREASETRACK);

	Bind(VK_F9,SWITCH_TOD);

	done_with_credits=false;

	// Set command buffers to 0
	for (int counter=0;counter<32;counter++) cmdBuffer[counter]=0;
	// done setting cmdBuffers to 0

	MSG     msg;
	
	Dispatch(WINDOWCREATE);

	LoadGLTextures();
	BuildFont();

	for (int counter=0;counter<40;counter++)
	CannonBall[counter]=new cannonball();
	Bullet=new bullet();

	// Sound Stuff
	Sounds=new SoundManager(hWnd);
	LoadSounds();
	Sounds->play(STORY_NARRATION);
	AddCommand(KEY_INTROMUSIC);
	credit_stage=false;
	// Load Textures
	// Compile Scene
	CompileOriginalScene();
	DefaultView=new Camera();
	gunMovement[0]=true;
	gunMovement[1]=true;

	game_started=false;
	game_over=false;
	won=false;

	gunWobble.x=0.0;
	gunWobble.y=0.0;

	seedrnd();

	mouseprevx=512;
	mouseprevy=384;

	while (app_StillRunning)
	{
		if (PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if (msg.message==WM_QUIT)
			{
				app_StillRunning=false;
			}
			else
			{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
			}
		}
		else
		{
			if (app_Active)
			{
				// Dispatch all messages
				walking=false;
				for (int counter=0;counter<16;counter++)
					Dispatch(cmdBuffer[counter]);

				//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				glClear(GL_DEPTH_BUFFER_BIT);
				glLoadIdentity();
				DefaultView->LookAt();
				DrawScene();

				// Printing Game Text
				glColor4d(1.0-timeofdaycolors[timeofday][0],1.0-timeofdaycolors[timeofday][1],1.0-timeofdaycolors[timeofday][2],timeofdaycolors[timeofday][3]);
				if (!game_started) glPrint(320,730,1,"Press [F4] for a new game");
				if (game_started)
				{
					glPrint(0,730,1,"-> Very End  of song list");
					glPrint(0,700,1,"<- Begininng of song list");
					glPrint(501,373,1,"+");
					DefaultView->drawscore();
				}
				if (game_over)
				{
					app_StillRunning=false;
					credit_stage=true;
				}
				if (credit_stage)
				{
					CreditTime->start();;
					Sounds->stop();
					for (;;)
					{
						CreditTime->update();
						if (CreditTime->getElapsedS() >0.0025)
						{
							CreditTime->start();
							i--;
						}
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
						glLoadIdentity();
						glColor4d(1.0,1.0,1.0,1.0);
						if (!won)
						{
						glPrint(0,i,1,"                             You Are Dead!!!");
						glPrint(0,i+30,1,"                   The Queen Reigns Over Your Family.");
						}
						else
						{
						glPrint(0,i,1,"                               Great Job!!!");
						glPrint(0,i+30,1,"           You can now overthrow the queen and save the world.");
						}
						glColor4d(1.0,0.0,0.0,1.0);
						glPrint(0,i+90,1,"                   Jerrod and Jon would like to thank:");
						glColor4d(1.0,0.0,1.0,1.0);
						glPrint(0,i+120,1,"          GOD (for talent, and in other aspects of our lives)");
						glPrint(0,i+150,1,"           Robert Miller (for making this an easy project) ;-)");
						glPrint(0,i+180,1,"                      NeHe (for OpenGL Tutorials)");
						glPrint(0,i+210,1,"                    Sierra's Halflife (for sound FX)");
						glPrint(0,i+240,1,"               ID software's Quake (for sound FX && ideas)");
						glPrint(0,i+270,1,"                 Ian Parberry (for DirectSound Tutorials)");
						glColor4d(0.0,0.0,1.0,1.0);
						glPrint(0,i+330,1,"                 Each other for hard work and dedication.");
						SwapBuffers(hDC);
						if (i<=-400)
							break;
						if (done_with_credits)
							break;
					}
				}


				glColor4d(timeofdaycolors[timeofday][0],timeofdaycolors[timeofday][1],timeofdaycolors[timeofday][2],timeofdaycolors[timeofday][3]);
				SwapBuffers(hDC);
			}
		}
	}
	Dispatch(WINDOWDESTROY);
	return (int(msg.wParam));
}

