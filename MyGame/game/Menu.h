#pragma once

#include "iStd.h"

extern iPopup* popStage;

///////////////////////////////////////
// Menu
///////////////////////////////////////

void loadMenu();
void freeMenu();
void drawMenu(float dt);
void keyMenu(iKeyState stat, iPoint point);

///////////////////////////////////////
// PopMenu
///////////////////////////////////////

void loadPopMenu();
void freePopMenu();
void drawPopMenu(float dt);
void showPopMenu(bool show);
bool keyPopMenu(iKeyState stat, iPoint point);


