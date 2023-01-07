// Dispatch.cpp

#ifndef APP_NAME
#define APP_NAME L"Wall of Balls 1.0"
#endif

#include <windows.h>
#include <gl/gl.h>

#include "GlobalEnum.h"
#include "sound.h"
#include "Camera.h"
#include "cannonball.h"
#include "SaveDIBitmap.h"

#include "bullet.h"

////2013
#include "System.h"
#include "Game.h"

extern GLuint MAX_SCREENMODES;
extern GLuint MAX_SCREENBPP;

extern GLuint app_ScreenModes[3][2];
extern GLuint app_ScreenBPP[2];

extern GLuint CURRENT_SCREENMODE;
extern GLuint CURRENT_SCREENBPP;
extern SoundManager* Sounds;


int tracknumber = 0;
/////////////////////////////////////////////////////////////////////////////////////////


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
		if (G.app_StillRunning==false)
			G.done_with_credits=true;
		G.app_StillRunning=false;
		S.RemoveCommand(EXIT);
		return true;
		break;
	case SWITCH_SCREENMODE:
		//S.FreeWindow();
		//S.FreeDriver();

		S.RemoveCommand(SWITCH_SCREENMODE);
		S.ForceKey(SWITCH_SCREENMODE, false);

		CURRENT_SCREENMODE++;
		if (CURRENT_SCREENMODE==MAX_SCREENMODES)
			CURRENT_SCREENMODE=0;
		//if (!CreateGLWindow(APP_NAME,
		//					app_ScreenModes[CURRENT_SCREENMODE][0],
		//					app_ScreenModes[CURRENT_SCREENMODE][1],
		//					app_ScreenBPP[CURRENT_SCREENBPP],
		//					app_Fullscreen))
		//	return false;
		//S.InitWindow(app_ScreenModes[CURRENT_SCREENMODE][0], app_ScreenModes[CURRENT_SCREENMODE][1], app_ScreenBPP[CURRENT_SCREENBPP], app_Fullscreen);
		//S.InitDriver();
		//InitGL();
		//ReSizeGLScene(app_ScreenModes[CURRENT_SCREENMODE][0], app_ScreenModes[CURRENT_SCREENMODE][1]);
		//G.CompileScene();
		return true;
		break;
	case SWITCH_SCREENBPP:
		//KillGLWindow();
		S.RemoveCommand(SWITCH_SCREENBPP);
		S.ForceKey(SWITCH_SCREENBPP, false);

		CURRENT_SCREENBPP++;
		if (CURRENT_SCREENBPP==MAX_SCREENBPP)
			CURRENT_SCREENBPP=0;
		//if (!CreateGLWindow(APP_NAME,
		//					app_ScreenModes[CURRENT_SCREENMODE][0],
		//					app_ScreenModes[CURRENT_SCREENMODE][1],
		//					app_ScreenBPP[CURRENT_SCREENBPP],
		//					app_Fullscreen))
		//	return false;
		//LoadGLTextures();
		//G.CompileScene();
		return true;
		break;
	case SWITCH_FULLSCREEN:
		//KillGLWindow();
		S.RemoveCommand(SWITCH_FULLSCREEN);
		S.ForceKey(SWITCH_FULLSCREEN, false);
		S.WindowFullScreen=!S.WindowFullScreen;
		//if (!CreateGLWindow(APP_NAME,
		//					app_ScreenModes[CURRENT_SCREENMODE][0],
		//					app_ScreenModes[CURRENT_SCREENMODE][1],
		//					app_ScreenBPP[CURRENT_SCREENBPP],
		//					app_Fullscreen))
		//	return false;
		//G.CompileScene();
		if (S.WindowFullScreen)
			ShowCursor(false);
		else
			ShowCursor(true);
		return true;
		break;
	case WINDOWCREATE:
		//if (!CreateGLWindow(APP_NAME,
		//					app_ScreenModes[CURRENT_SCREENMODE][0],
		//					app_ScreenModes[CURRENT_SCREENMODE][1],
		//					app_ScreenBPP[CURRENT_SCREENBPP],
		//					app_Fullscreen))
		//	return false;
		//InitGL();
		//ReSizeGLScene(app_ScreenModes[CURRENT_SCREENMODE][0], app_ScreenModes[CURRENT_SCREENMODE][1]);
		//LoadGLTextures();
		//G.CompileScene();
		return true;
		break;
	case WINDOWDESTROY:
		//KillGLWindow();
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
				S.RemoveCommand(KEY_INTROMUSIC);
				Sounds->stopsound(INTROMUSIC_SOUND);
				Sounds->stopsound(STORY_NARRATION);
				Sounds->play(tracknumber);
				S.AddCommand(CURRENT_GAMEMUSIC);
				G.game_started=true;

				//for (counter=0;counter<40;counter++)
				//G.CannonBall[counter]->deploy();
				for (counter=0;counter<40;counter++)
					G.cannonBallRender[counter].cb->deploy();

			break;
			case SWITCH_TOD:
				S.RemoveCommand(SWITCH_TOD);
				G.timeofday++;
				if (G.timeofday>2)
					G.timeofday=0;
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
			G.DefaultView->Advance(1.0);
			G.walking=true;
			break;
		case MOVE_BACKWARD:
			G.DefaultView->Advance(-1.0);
			G.walking=true;
			break;
		case MOVE_LEFT:
			G.DefaultView->Strafe(1.0);
			G.walking=true;
			break;
		case MOVE_RIGHT:
			G.DefaultView->Strafe(-1.0);
			G.walking=true;
			break;
		case LOOK_LEFT:
			G.DefaultView->LookSideWays(-1.0);
			break;
		case LOOK_RIGHT:
			G.DefaultView->LookSideWays(1.0);
			break;
		case LOOK_UP:
			G.DefaultView->LookUp(-1.0);
			break;
		case LOOK_DOWN:
			G.DefaultView->LookUp(1.0);
			break;
		case JUMP:
			G.DefaultView->Jump();
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
				G.Bullet->deploy();
			break;

			case KEY_DECREASETRACK:
				if (G.game_started == true)
				{
					S.RemoveCommand(KEY_DECREASETRACK);
					if (tracknumber > 0)
					{
						Sounds->stopsound(tracknumber);
						tracknumber = tracknumber - 1;
						Sounds->play(tracknumber);
					}				
				}
			
			break;

			case KEY_INCREASETRACK:
				if (G.game_started == true)
				{
					S.RemoveCommand(KEY_INCREASETRACK);			
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
