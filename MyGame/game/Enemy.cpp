#include "Enemy.h"

#include "Common.h"
//#include "Proc.h"
#include "Player.h"

Texture** texEnemies;
Enemy* _enemy;
Enemy** enemy;
int numEnemy;

METHOD_GET_SCORE methodGetScore;
void methodEnemyAi(Enemy* e, float dt, iPoint off);

void loadEnemy(METHOD_GET_SCORE methodScore)
{
	Texture* tex;
	texEnemies = (Texture**)malloc(sizeof(Texture*)*numTextureEnemy);
	for (int i = 0; i < numTextureEnemy; i++)
	{
		tex = createImage("assets/textures/enemy/fishTile_%d.png", i);
		texEnemies[i] = tex;
	}

	//texEnemy = texEnemies[78];
	_enemy = (Enemy*)calloc(sizeof(Enemy), _numEnemy);
	enemy = (Enemy**)malloc(sizeof(Enemy*)*_numEnemy);

	numEnemy = 0;

	methodGetScore = methodScore;
}

void freeEnemy()
{
	for (int i = 0; i < numTextureEnemy; i++)
		freeImage(texEnemies[i]);

	free(texEnemies);

	free( _enemy );
	free( enemy );

	
}

void drawEnemy(float dt, iPoint off)
{
	for(int i = 0 ; i < numEnemy ; i++)
	{
		if(enemy[i]->paint(dt, off))
		{
			numEnemy--;
			enemy[i] = enemy[numEnemy];
			i--;
		}
	}
}

void addEnemy(iPoint sp)
{
	Enemy* e;
	for(int i = 0 ; i < _numEnemy ; i++)
	{
		e = &_enemy[i];
		if( !e->alive )
		{
			e->hp = e->_hp = 10;
			e->ap = e->_ap = 1;
			e->point = 10;
			e->speed = e->_speed = 150;
			e->damageTime = _damageTime;
			e->position = sp;
			e->vector = iPointZero;
			e->alive = true;
			e->method = methodEnemyAi;
			e->tex = texEnemies[random() % numTextureEnemy];

			enemy[numEnemy] = e;
			numEnemy++;
			break;
		}
	}
}

iRect getEnemyRect(Enemy* e)
{
	iPoint p = e->position;
	iSize size = enemySize;
	return iRectMake(p.x-size.width/2, p.y-size.height/2, size.width, size.height);
}

bool Enemy::paint(float dt, iPoint off)
{
	iPoint p = position + off;

	if( !alive )
		return true;

	if( p.x + 50 < 0 )
	{
		alive = false;
		return true;
	}

	method(this, dt, off);

	p = position + off;

	float degree = iPointAngle(iPointMake(-1, 0), iPointZero, vector);

	drawImage(tex,
			  p.x, p.y,
			  0, 0, tex->width, tex->height,
			  1.0f, 1.0f,
			  2, degree,
			  VCENTER|HCENTER, 1);

	if (damageTime < _damageTime/4)
	{
		damageTime += dt;

		setRGBA(0, 0, 1, fabsf(_sin(180 * damageTime / _damageTime/4)));
		drawImage(tex,
			p.x, p.y,
			0, 0, tex->width, tex->height,
			1.0f, 1.0f,
			2, degree,
			VCENTER | HCENTER, 1);
		setRGBA(1, 1, 1, 1);
	}

#if 0 // 충돌 영역 표시
	setRGBA(0, 1, 0, 0.5);
	fillRect(getEnemyRect(this));
	setRGBA(1, 1, 1, 1);
#endif

	if (containRect(getEnemyRect(this), getPlayerRect()))
	{
		player->getDamage(1);
		alive = false;
		return true;
	}

	return false;
}

void Enemy::getDamage(int damage)
{
	if(hp <= 0)
	{
		audioPlay(snd_enemyDown);
		if (methodGetScore)
			methodGetScore(point);
		
		alive = false;
		return;
	}
	damageTime = 0.0f;
	hp -= damage;
}


void methodEnemyAi(Enemy* e, float dt, iPoint off)
{
	iPoint posPlayer, posEnemy, vectorEnemy;
	float speed;

	speed = e->speed;
	posPlayer = player->position;
	posEnemy = e->position;
	vectorEnemy = iPointMake(-1, 0);

	posEnemy += vectorEnemy*(speed*dt);
	e->position = posEnemy;
	e->vector = vectorEnemy;
}

