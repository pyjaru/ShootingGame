#pragma once

#include "iStd.h"

#define numTextureEnemy 5

struct Enemy;
typedef void (*METHOD_ENEMY_AI)(Enemy* e, float dt, iPoint off);
typedef void(*METHOD_GET_SCORE)(int score);

void loadEnemy(METHOD_GET_SCORE methodScore = NULL);
void freeEnemy();
void drawEnemy(float dt, iPoint off);
void addEnemy(iPoint sp);
iRect getEnemyRect(Enemy* e);

struct Enemy
{
	int hp, _hp;
	int ap, _ap;
	int point;
	float speed, _speed;
	float damageTime;
	Texture* tex;
	iPoint position;
	iPoint vector;
	bool alive;

	bool paint(float dt, iPoint off);
	void getDamage(int damage);

	METHOD_ENEMY_AI method;
};

#define enemySize iSizeMake(64, 64)
#define _numEnemy 128

extern int numEnemy;
extern Enemy** enemy;
