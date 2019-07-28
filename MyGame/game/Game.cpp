#include "Game.h"

#include "Common.h"
#include "Loading.h"

#include "Intro.h"
#include "Menu.h"
#include "Proc.h"

#include "Player.h"


void loadGame()
{
	createSound();
	loadAppData();
	loadIntro();
	
	loadPlayer();

	gameState = gs_intro;
}

void freeGame()
{
	switch(gameState)
	{
		case gs_intro:		freeIntro();	break;
		case gs_menu:		freeMenu();		break;
		case gs_proc:		freeProc();		break;
		default: break;
	}

	audioStop();

	freePlayer();
	freeAppData();
	freeSound();
	freePopExit();
}

void drawGame(float dt)
{
	float _dt = dt;
	if( popExit && popExit->bShow )
		dt = 0.0f;

	switch(gameState)
	{
		case gs_intro:		drawIntro(dt);		break;
		case gs_menu:		drawMenu(dt);		break;
		case gs_proc:		drawProc(dt);		break;
		default: break;
	}

	drawLoading(dt);

	drawPopExit(_dt);
}

void keyGame(iKeyState stat, iPoint point)
{
	if( keyPopExit(stat, point) )
		return;

	switch(gameState)
	{
		case gs_intro:		keyIntro(stat, point); break;
		case gs_menu:		keyMenu(stat, point); break;
		case gs_proc:		keyProc(stat, point); break;
		default:break;
	}
}



