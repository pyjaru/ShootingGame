#pragma once

#include "iStd.h"

typedef void (*LOADING_METHOD)();

typedef enum _GameState {
	gs_intro  = 0,
	gs_menu,
	gs_proc,
	gs_credit,
	gs_loading,
} GameState;

extern GameState gameState;

void setLoading(GameState gs, LOADING_METHOD methodFree, LOADING_METHOD methodLoad);
void drawLoading(float dt);

#define _loadingDt 0.3f
