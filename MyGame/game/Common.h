#pragma once

#include "iStd.h"

void drawPopCommon(iPopup* pop, float alpha);

///////////////////////////////////////
// PopSettings
///////////////////////////////////////
typedef void (*CLOSE_SETTING_METHOD)(bool result);

void loadPopSetting(CLOSE_SETTING_METHOD method = NULL);
void freePopSetting();
void drawPopSetting(float dt);
void showPopSetting(bool show);
bool keyPopSetting(iKeyState stat, iPoint point);
bool isSettingShow();

//////////////////////////////////////////////////////
// PopExit
//////////////////////////////////////////////////////
extern iPopup* popExit;

void createPopExit();
void freePopExit();
void showPopExit(bool show);
void drawPopExit(float dt);
bool keyPopExit(iKeyState stat, iPoint point);

//////////////////////////////////////////////////////
// Font
//////////////////////////////////////////////////////
class iFont
{
public:
	iFont();
	virtual ~iFont();

public:
	iSize sizeOfString(float rate, const char* szFormat, ...);
	void drawString(float x, float y, int anc, float rate, const char* szFormat, ...);

public:
	Texture** texs;
	float gap;
};

//////////////////////////////////////////////////////
// Sound
//////////////////////////////////////////////////////
enum {
	snd_btnSwitch = 0,
	snd_click,
	snd_enemyDown,
	snd_introClick,
	snd_shoot,

	snd_mainMenu,
	snd_inGame,

	snd_totalnum,
};

void createSound();
void freeSound();

//////////////////////////////////////////////////////
//   Save 및 Stage 데이터
//////////////////////////////////////////////////////

struct AppData {
	int bestScore;
	float bgm, eff;
};

#define APP_FILE "app.sav"
extern AppData* appData;

void freeAppData();
void loadAppData();
void saveAppData();
void renewScore(int score);