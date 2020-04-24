// Dispatch.cpp

#ifndef APP_NAME
#define APP_NAME "Wall of Balls 1.0"
#endif

#include <windows.h>
#include <gl/gl.h>

#include "GlobalEnum.h"
#include "sound.h"
#include "Camera.h"
#include "cannonball.h"
#include "SaveDIBitmap.h"

#include "bullet.h"
extern bullet *Bullet;

extern int timeofday;

extern cannonball *CannonBall[40];

//Function Declarations
/////////////////////////////////////////////////////////////////////////////////////////
bool CreateGLWindow(char* title,int width,int height,int bits, bool fullscreenflag);
GLvoid KillGLWindow(GLvoid);
void CompileScene(void);
int LoadGLTextures();
/////////////////////////////////////////////////////////////////////////////////////////

//Variable declarations
/////////////////////////////////////////////////////////////////////////////////////////
extern bool app_StillRunning;
extern bool app_Fullscreen;

extern GLint cmdBuffer[32];
extern bool Keys[256];
extern GLuint KeyMessage[256];

extern GLuint MAX_SCREENMODES;
extern GLuint MAX_SCREENBPP;

extern GLuint app_ScreenModes[3][2];
extern GLuint app_ScreenBPP[2];

extern GLuint CURRENT_SCREENMODE;
extern GLuint CURRENT_SCREENBPP;
extern SoundManager* Sounds;

extern bool game_started;

extern Camera *DefaultView;

extern bool walking;
extern int GamePhase;
extern bool done_with_credits;

int tracknumber = 0;
/////////////////////////////////////////////////////////////////////////////////////////

bool AddCommand(int Message)
{
	bool hit=false;
	for (int counter=0;counter<16;counter++)
	{
		if (Message==cmdBuffer[counter])
			hit=true;
	}
	if (hit==false)
	for (int counter=0;counter<16;counter++)
	{
		if (cmdBuffer[counter]==0)
		{
			cmdBuffer[counter]=Message;
			break;
		}
	}	
	return true;
}

bool RemoveCommand(int Message)
{
	for (int counter=0;counter<16;counter++)
	{
		if (cmdBuffer[counter]==Message)
		{
			cmdBuffer[counter]=0;
			break;
		}
	}
	return true;
}

bool Dispatch(int msgInput)
{
	int counter;
	// Graphics Messages
	if (msgInput>299 && msgInput<400)
	{
	switch (msgInput)
	{

	case 0:
		break;
	case EXIT:
		if (app_StillRunning==false)
			done_with_credits=true;
		app_StillRunning=false;
		RemoveCommand(EXIT);
		return true;
		break;
	case SWITCH_SCREENMODE:
		KillGLWindow();
		RemoveCommand(SWITCH_SCREENMODE);
		for (counter=0;counter<256;counter++)
			if (KeyMessage[counter]==SWITCH_SCREENMODE)
				Keys[counter]=false;
		CURRENT_SCREENMODE++;
		if (CURRENT_SCREENMODE==MAX_SCREENMODES)
			CURRENT_SCREENMODE=0;
		if (!CreateGLWindow(APP_NAME,
							app_ScreenModes[CURRENT_SCREENMODE][0],
							app_ScreenModes[CURRENT_SCREENMODE][1],
							app_ScreenBPP[CURRENT_SCREENBPP],
							app_Fullscreen))
			return false;
		LoadGLTextures();
		CompileScene();
		return true;
		break;
	case SWITCH_SCREENBPP:
		KillGLWindow();
		RemoveCommand(SWITCH_SCREENBPP);
		for (counter=0;counter<256;counter++)
			if (KeyMessage[counter]==SWITCH_SCREENBPP)
				Keys[counter]=false;
		CURRENT_SCREENBPP++;
		if (CURRENT_SCREENBPP==MAX_SCREENBPP)
			CURRENT_SCREENBPP=0;
		if (!CreateGLWindow(APP_NAME,
							app_ScreenModes[CURRENT_SCREENMODE][0],
							app_ScreenModes[CURRENT_SCREENMODE][1],
							app_ScreenBPP[CURRENT_SCREENBPP],
							app_Fullscreen))
			return false;
		LoadGLTextures();
		CompileScene();
		return true;
		break;
	case SWITCH_FULLSCREEN:
		KillGLWindow();
		RemoveCommand(SWITCH_FULLSCREEN);
		for (counter=0;counter<256;counter++)
			if (KeyMessage[counter]==SWITCH_FULLSCREEN)
				Keys[counter]=false;
		app_Fullscreen=!app_Fullscreen;
		if (!CreateGLWindow(APP_NAME,
							app_ScreenModes[CURRENT_SCREENMODE][0],
							app_ScreenModes[CURRENT_SCREENMODE][1],
							app_ScreenBPP[CURRENT_SCREENBPP],
							app_Fullscreen))
			return false;
		LoadGLTextures();
		CompileScene();
		if (app_Fullscreen)
			ShowCursor(false);
		else
			ShowCursor(true);
		return true;
		break;
	case WINDOWCREATE:
		if (!CreateGLWindow(APP_NAME,
							app_ScreenModes[CURRENT_SCREENMODE][0],
							app_ScreenModes[CURRENT_SCREENMODE][1],
							app_ScreenBPP[CURRENT_SCREENBPP],
							app_Fullscreen))
			return false;
		LoadGLTextures();
		CompileScene();
		return true;
		break;
	case WINDOWDESTROY:
		KillGLWindow();
		return true;
		break;
	case SAVE_SCREEN:
		SaveDIB("ScreenShot.bmp");
		return true;
		break;
	default:
		break;
	}
	}
	// Menu Messages
	if (msgInput>399 && msgInput<499)
	{
		switch (msgInput)
		{
			case NEW_GAME:
				RemoveCommand(KEY_INTROMUSIC);
				Sounds->stopsound(INTROMUSIC_SOUND);
				Sounds->stopsound(STORY_NARRATION);
				Sounds->play(tracknumber);
				AddCommand(CURRENT_GAMEMUSIC);
				game_started=true;

				for (counter=0;counter<40;counter++)
				CannonBall[counter]->deploy();

			break;
			case SWITCH_TOD:
				RemoveCommand(SWITCH_TOD);
				timeofday++;
				if (timeofday>2)
					timeofday=0;
			break;

		}
		
	}
	// Game Messages
	if (msgInput>499 && msgInput<599)
	{
		switch(msgInput)
		{
		// Camera Movement
		case MOVE_FOREWARD:
			DefaultView->Advance(1.0);
			walking=true;
			break;
		case MOVE_BACKWARD:
			DefaultView->Advance(-1.0);
			walking=true;
			break;
		case MOVE_LEFT:
			DefaultView->Strafe(1.0);
			walking=true;
			break;
		case MOVE_RIGHT:
			DefaultView->Strafe(-1.0);
			walking=true;
			break;
		case LOOK_LEFT:
			DefaultView->LookSideWays(-1.0);
			break;
		case LOOK_RIGHT:
			DefaultView->LookSideWays(1.0);
			break;
		case LOOK_UP:
			DefaultView->LookUp(-1.0);
			break;
		case LOOK_DOWN:
			DefaultView->LookUp(1.0);
			break;
		case JUMP:
			DefaultView->Jump();
			break;
		default:
			break;
		}
	}
	// Sound Messages
	if (msgInput>599 && msgInput<699)
	{
		switch (msgInput)
		{
			case KEY_INTROMUSIC:
				Sounds->play(INTROMUSIC_SOUND);
			break;


			case KEY_FIREGUN:
				Sounds->play(FIREGUN_SOUND);
				Bullet->deploy();
			break;

			case KEY_DECREASETRACK:
				if (game_started == true)
				{
					RemoveCommand(KEY_DECREASETRACK);
					if (tracknumber > 0)
					{
						Sounds->stopsound(tracknumber);
						tracknumber = tracknumber - 1;
						Sounds->play(tracknumber);
					}				
				}
			
			break;

			case KEY_INCREASETRACK:
				if (game_started == true)
				{
					RemoveCommand(KEY_INCREASETRACK);			
					if (tracknumber < 6)
					{
						Sounds->stopsound(tracknumber);
						tracknumber = tracknumber + 1;
						Sounds->play(tracknumber);
					}
				}
			
				break;

			case CURRENT_GAMEMUSIC:
				Sounds->play(tracknumber);
			break;


		}

	}
	return true;
}
