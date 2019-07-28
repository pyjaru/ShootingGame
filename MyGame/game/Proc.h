#pragma once

#include "iStd.h"

#define playWaitTime 3f

void loadProc();
void freeProc();
void drawProc(float dt);
void keyProc(iKeyState stat, iPoint point);

///////////////////////////////////////////
// PopProc
///////////////////////////////////////////

//extern iPopup* popProc;

void loadPopProc();
void freePopProc();
void drawPopProc(float dt);
bool keyPopProc(iKeyState stat, iPoint point);
void showPopProc(bool show);

///////////////////////////////////////////
// PopResult
///////////////////////////////////////////

extern iPopup* popResult;

void loadPopResult();
void freePopResult();
void drawPopResult(float dt);
bool keyPopResult(iKeyState stat, iPoint point);
void showPopResult(bool show);

///////////////////////////////////////////
// PopPause
///////////////////////////////////////////

extern iPopup* popPause;

void loadPopPause();
void freePopPause();
void drawPopPause(float dt);
bool keyPopPause(iKeyState stat, iPoint point);
void showPopPause(bool show);
