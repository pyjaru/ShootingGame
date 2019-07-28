#include "Player.h"

#include "loading.h"
#include "Common.h"
#include "Bullet.h"
#include "Map.h"

Player* player;
iPoint mapBoundOff;
iRect mapBound;

void loadPlayer()
{
	Texture* tex;
	iImage* img;
	int i = 0;

	img = new iImage();
	for (i = 0; i < 6; i++)
	{
		tex = createImage("assets/textures/player/Dolphin_%d.png", i);
		img->addObject(tex);
		freeImage(tex);
	}
	img->position = iPointMake(devSize.width / 2, devSize.height / 2);
	img->_intervalDt = 0.2;
	img->startAnimation(NULL);

	player = new Player(img);
	
	iSize size = player->size;	

	mapBoundOff = iPointMake(10, 10);
	mapBound = iRectMake(mapBoundOff.x, mapBoundOff.y,
		devSize.width - 100 - 2 * mapBoundOff.x - size.width,
		devSize.height - 2 * mapBoundOff.y - size.height);
}

void freePlayer()
{
	delete player;
}

void drawPlayer(float dt, iPoint point)
{
	player->paint(dt, point);
}

iRect getPlayerRect()
{
	iPoint p = player->position;
	iSize size = player->size;

	// 충돌 영역을 돌고래 머리에서 몸통부분으로 조정
	return iRectMake(p.x+80, p.y+50, size.width-130, size.height-80);
}

Player::Player(iImage* img)
{
	this->img = img;
	this->size = iSizeMake(img->tex->width, img->tex->height);

	ready();
}

Player::~Player()
{
	if (img)
		delete img;
}

void Player::ready()
{
	// 플레이 모드 기준으로 셋팅
	hp = _hp = _playerDefaultLife;
	ap = _ap = 2;
	speed = _speed = 500;
	fireDelay = 0.0f;
	damageTime = _damageTime;
	position = iPointMake(-devSize.width / 2, devSize.height / 2);
	moveVector = iPointZero;
	alive = true;

	img->leftRight = 0; //이미지 좌우 반전 초기화
	img->position = iPointZero;
	img->setTexAtIndex(0);
}

void Player::paint(float dt, iPoint off)
{
	if (gameState == gs_proc)
	{
		update(dt, off);

		iPoint p = off + player->position;
		
		img->paint(dt, p);

		// 피격 효과
		if (damageTime < _damageTime)
		{
			float r = damageTime / _damageTime;
			damageTime += dt;

			setRGBA(1, 0, 0, fabs(_sin(360 * r)));
			drawImage(img->tex, p.x, p.y, TOP | LEFT);
			setRGBA(1, 1, 1, 1);
		}
	}
	else
	{
		iPoint v = off - (img->position + iPointMake(size.width / 2, size.height / 2));
		img->position += v * dt;
		
		img->paint(dt);
	}

#if 0 // 충돌 영역 표시
		setRGBA(1, 0, 0, 0.5);
		fillRect(getPlayerRect());
		setRGBA(1, 1, 1, 1);
#endif
}

void Player::update(float dt, iPoint off)
{
	iPoint v = moveVector;

	float po = dt;
	if( v.x > 0 && (getKeyStat()&keyboard_right)!=keyboard_right )		{ v.x -= po; if( v.x < 0 ) v.x = 0; }
	else if( v.x < 0 && (getKeyStat()&keyboard_left)!=keyboard_left  )	{ v.x += po; if( v.x > 0 ) v.x = 0; }
	if( v.y > 0 && (getKeyStat()&keyboard_down)!=keyboard_down )		{ v.y -= po; if( v.y < 0 ) v.y = 0; }
	else if( v.y < 0 && (getKeyStat()&keyboard_up)!=keyboard_up )		{ v.y += po; if( v.y > 0 ) v.y = 0; }

	po = dt*5;
	switch(getKeyStat()&15)	// getKeyStat() & (keyboard_left|keyboard_right|keyboard_up|keyboard_down) //00011111
	{
		case keyboard_left:                v.x -= po; if( v.x<-1 ) v.x = -1; break;
		case keyboard_left|keyboard_up:    v.x -= po; if( v.x<-1 ) v.x = -1; v.y -= po; if( v.y<-1 ) v.y = -1; break;
		case keyboard_left|keyboard_down:  v.x -= po; if( v.x<-1 ) v.x = -1; v.y += po; if( v.y> 1 ) v.y =  1; break;
		case keyboard_right:               v.x += po; if( v.x> 1 ) v.x =  1; break;
		case keyboard_right|keyboard_up:   v.x += po; if( v.x> 1 ) v.x =  1; v.y -= po; if( v.y<-1 ) v.y = -1; break;
		case keyboard_right|keyboard_down: v.x += po; if( v.x> 1 ) v.x =  1; v.y += po; if( v.y> 1 ) v.y =  1; break;
		case keyboard_up:                  v.y -= po; if( v.y<-1 ) v.y = -1; break;
		case keyboard_down:                v.y += po; if( v.y> 1 ) v.y =  1; break;
		default:break;
	}

	if(v != iPointZero)
	{
		float speedRate = iPointLength(v);

		if(speedRate > speed)
			v = v*(speed/speedRate);

		position += v*(speed*dt);

		if (!containPoint(position, mapBound))
		{
			// Check horizontal bound
			if (position.x < mapBoundOff.x)
			{
				v.x = 0;
				position.x = mapBoundOff.x;
			}
			else if (position.x > mapBoundOff.x + mapBound.size.width)
			{
				v.x = 0;
				position.x = mapBoundOff.x + mapBound.size.width;
			}
			
			// Check vertical bound
			if (position.y < mapBoundOff.y)
			{
				v.y = 0;
				position.y = mapBoundOff.y;
			}
			else if(position.y > mapBoundOff.y + mapBound.size.height)
			{
				v.y = 0;
				position.y = mapBoundOff.y + mapBound.size.height;
			}

		}
		
		moveVector = v;
	}

	if( getKeyStat()&keyboard_space)
	{
		fireDelay += dt;
		while(fireDelay > _playerFireDelay)
		{
			attack();
			fireDelay -= _playerFireDelay;
		}
	}
}

void Player::attack()
{
	if (!alive)
		return;

	audioPlay(snd_shoot);

	iPoint p = position + iPointMake(size.width, size.height/2);
	addBullet(this, 1, p, iPointMake(p.x+1, p.y), iSizeMake(16,32));
}

void Player::getDamage(int damage)
{
	if(hp <= 0)
	{
		alive = false;
		return;
	}
	hp -= damage;

	damageTime = 0.0f;
}

void Player::setImgReverse(iPoint point)
{
	iPoint off = iPointMake(size.width / 2, size.height / 2);
	iPoint sp = img->position + off;
	iPoint ep = point;

	img->leftRight = ((ep.x - sp.x) < 0);
}
