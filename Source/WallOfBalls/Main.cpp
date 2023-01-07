#define APP_NAME "Wall of Balls 1.0"

#include <Windows.h>
#include <gl/gl.h>

#include "Font.h"

/////2013
#include "System.h"
#include "Game.h"
#include "Sound.h"

extern SoundManager* Sounds;

void Game::Exec(void)
{
	while (app_StillRunning)
	{
		walking = false;

		S.ProcMessages();
		
		DriverGL11::Print(0, 0, 1, "");
		DriverGL11::Clear(GL_DEPTH_BUFFER_BIT);
		DriverGL11::LoadIdentity();
		DefaultView->LookAt();
		G.DrawScene();

		// Printing Game Text
		DriverGL11::Color4d(1.0-timeofdaycolors[timeofday][0],1.0-timeofdaycolors[timeofday][1],1.0-timeofdaycolors[timeofday][2],timeofdaycolors[timeofday][3]);
		if (!game_started) 
			DriverGL11::Print(320,730,1,"Press [F4] for a new game");
		if (game_started)
		{
			DriverGL11::Print(0, 730, 1, "-> Very End  of song list");
			DriverGL11::Print(0, 700, 1, "<- Begininng of song list");
			DriverGL11::Print(501, 373, 1, "+");
			DefaultView->drawscore();
		}
		if (game_over)
		{
			app_StillRunning=false;
			credit_stage=true;
		}
		if (credit_stage)
		{
			CreditTime->start();
			Sounds->stop();
			for (;;)
			{
				CreditTime->update();
				if (CreditTime->getElapsedS() >0.0025)
				{
					CreditTime->start();
					i--;
				}
				DriverGL11::Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
				DriverGL11::LoadIdentity();
				DriverGL11::Color4d(1.0,1.0,1.0,1.0);
				if (!won)
				{
					DriverGL11::Print(0,i,1,"                             You Are Dead!!!");
					DriverGL11::Print(0,i+30,1,"                   The Queen Reigns Over Your Family.");
				}
				else
				{
					DriverGL11::Print(0,i,1,"                               Great Job!!!");
					DriverGL11::Print(0,i+30,1,"           You can now overthrow the queen and save the world.");
				}
				DriverGL11::Color4d(1.0,0.0,0.0,1.0);
				DriverGL11::Print(0,i+90,1,"                   Jerrod and Jon would like to thank:");
				DriverGL11::Color4d(1.0,0.0,1.0,1.0);
				DriverGL11::Print(0,i+120,1,"          GOD (for talent, and in other aspects of our lives)");
				DriverGL11::Print(0,i+150,1,"           Robert Miller (for making this an easy project) ;-)");
				DriverGL11::Print(0,i+180,1,"                      NeHe (for OpenGL Tutorials)");
				DriverGL11::Print(0,i+210,1,"                    Sierra's Halflife (for sound FX)");
				DriverGL11::Print(0,i+240,1,"               ID software's Quake (for sound FX && ideas)");
				DriverGL11::Print(0,i+270,1,"                 Ian Parberry (for DirectSound Tutorials)");
				DriverGL11::Color4d(0.0,0.0,1.0,1.0);
				DriverGL11::Print(0,i+330,1,"                 Each other for hard work and dedication.");
				SwapBuffers(DriverGL11::wglGetCurrentDC());
				if (i<=-400)
					break;
				if (done_with_credits)
					break;
			}
		}


		DriverGL11::Color4d(timeofdaycolors[timeofday][0],timeofdaycolors[timeofday][1],timeofdaycolors[timeofday][2],timeofdaycolors[timeofday][3]);
		SwapBuffers(DriverGL11::wglGetCurrentDC());
	}
}

int __stdcall WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	G.Init();
	G.Exec();
	G.Free();
	return 0;
}

