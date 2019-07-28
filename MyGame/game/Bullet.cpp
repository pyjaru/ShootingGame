#include "Bullet.h"

#include "Player.h"
#include "Enemy.h"

Texture* texBullet;

Bullet* _bullet;
Bullet** bullet;
int numBullet;

void playerBulletPattern(Bullet* b, float dt, iPoint position);

void loadBullet()
{
	Texture* tex;
	iImage* img = new iImage();

	_bullet = (Bullet*)calloc(sizeof(Bullet),_numBullet);
	bullet = (Bullet**)malloc(sizeof(Bullet*)*_numBullet);
	
	tex = createImage("assets/textures/player/wave.png");
	texBullet = tex;
}

void freeBullet()
{
	free( _bullet );
	free( bullet );

	freeImage(texBullet);
}

void drawBullet(float dt, iPoint off)
{
	Bullet* b;
	for(int i = 0 ; i < numBullet ; i++)
	{
		b = bullet[i];
		if( b->paint(dt, off) || b->hit() )
		{
			numBullet--;
			bullet[i] = bullet[numBullet];
			i--;
		}
	}
}

void addBullet(void* owner, int damage, iPoint sp, iPoint ep, iSize size)
{
	Bullet* b;
	for(int i = 0 ; i < _numBullet ; i++)
	{
		b = &_bullet[i];
		if( !b->active )
		{
			b->damage = 1;
			b->speed = 1000.0f;
			b->size = size;
			b->position = sp;
			b->vector = ep-sp;
			b->owner = owner;
			b->active = true;
			b->method = playerBulletPattern;

			bullet[numBullet] = b;
			numBullet++;
			break;
		}
	}
}

bool Bullet::paint(float dt, iPoint off)
{
	iPoint p = position + off;

	if(p.x < -64 || p.x > devSize.width+64 ||
	   p.y < -64 || p.y > devSize.height+64 )
	{
		active = false;
		return true;
	}

	method(this, dt, p);

	return false;
}

bool Bullet::hit()
{
	iRect rt = iRectMake(position.x, position.y, size.width, size.height);

	if(owner == player)
	{
		Enemy* e;
		for(int i = 0 ; i < numEnemy ; i++)
		{
			e = enemy[i];
			if( containRect(rt, getEnemyRect(enemy[i])) )
			{
				e->getDamage(damage);
				return true;
			}
		}
	}
	else
	{
		if( containRect(rt, getPlayerRect()) )
		{
			player->getDamage(damage);
			return true;
		}
	}

	return false;
}

void playerBulletPattern(Bullet* b, float dt, iPoint position)
{
	drawImage(texBullet, position.x, position.y, b->size.width, b->size.height, TOP|LEFT);

	b->position += b->vector*(b->speed * dt);
}
