#pragma once

#include "iStd.h"

void loadBullet();
void freeBullet();
void drawBullet(float dt, iPoint off);
void addBullet(void* owner, int damage, iPoint sp, iPoint ep, iSize size);

struct Bullet;
typedef void (*METHOD_BULLET_PATTERN)(Bullet* b, float dt, iPoint position);

struct Bullet
{
	int damage;
	float speed;
	iSize size;
	iPoint position;
	iPoint vector;
	void* owner;
	bool active;
	METHOD_BULLET_PATTERN method;

	bool paint(float dt, iPoint off = iPointZero);
	bool hit();
};

#define _numBullet 1000
#define playerBulletSize iSizeMake(16, 64)
