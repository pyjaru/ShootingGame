#pragma once

#include "iStd.h"

void loadPlayer();
void freePlayer();
void drawPlayer(float dt, iPoint point);
iRect getPlayerRect();

#define _damageTime 1.0f
#define _playerDefaultLife 3
#define playerSize iSizeMake(64,64)
#define _playerFireDelay 0.05f

class Player
{
public:
	Player(iImage* img);
	~Player();

public:
	void ready();
	void paint(float dt, iPoint off);
	void update(float dt, iPoint off);
	void attack();
	void getDamage(int damage);
	void setImgReverse(iPoint point);

public:
	int hp, _hp;
	int ap, _ap;
	float speed, _speed;
	float fireDelay;
	float damageTime;
	iSize size;
	iPoint position;
	iPoint moveVector;
	iImage* img;
	bool alive;
};

extern Player* player;


