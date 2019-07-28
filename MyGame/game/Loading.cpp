#include "Loading.h"

GameState gameState, toGameState;
LOADING_METHOD methodFree, methodLoad;

float loadingDt = 0.0f;

Texture* texLoading;
float alpha, scale;

void freeLoading()
{
	if( texLoading )
		freeImage( texLoading );
}

void setLoading(GameState gs, LOADING_METHOD _methodFree, LOADING_METHOD _methodLoad)
{
	toGameState = gs;
	methodFree = _methodFree;
	methodLoad = _methodLoad;
	loadingDt = 0.000001f;

	setStringSize(30);
	setStringRGBA(1,1,1,1);
	setStringBorder(5);
	setStringBorderRGBA(1,0,1,1);
	const char* str = "Loading";
	iSize strSize = sizeOfString(str);

	iGraphics* g = iGraphics::share();
	g->init(strSize);

	g->drawString(0, 0, TOP|LEFT, str);

	texLoading = g->getTexture();

	alpha = 1.0f;
	scale = 4.0f;
	gameState = gs_loading;
}

void drawLoading(float dt)
{
	if( loadingDt == 0.0f )
		return;

	if( loadingDt < _loadingDt )
	{
		scale +=(-3*dt);
		setRGBA(0.2f, 0.2f, 1, alpha);
		fillRect(0, 0, devSize.width, devSize.height);

		loadingDt += dt;
		if(loadingDt >= _loadingDt)
		{
			methodFree();

			gameState = toGameState;
			methodLoad();
		}
	}
	else if( loadingDt < _loadingDt*2 )
	{
		scale +=(3*dt);
		setRGBA(0.2f, 0.2f, 1, alpha);
		fillRect(0, 0, devSize.width, devSize.height);

		loadingDt += dt;
	}
	else if( loadingDt < _loadingDt*3 )
	{
		alpha = 1.0f-(loadingDt-_loadingDt*2)/_loadingDt;
		setRGBA(0.2f, 0.2f, 1, alpha);
		fillRect(0, 0, devSize.width, devSize.height);

		loadingDt += dt;
		if(loadingDt >= _loadingDt*3 )
		{
			loadingDt = 0.0f; // end
			freeLoading();
			setRGBA(1, 1, 1, 1);

			return;
		}
	}

	float _alpha = getAlpha();

	for(int j = 0 ; j < 14 ; j++)
	{
		for(int i = 0 ; i < 25; i++)
		{
			setRGBA(random()%256/255.f, random()%256/255.f, random()%256/255.f, alpha);
			fillRect(64*i, 64*j, 64, 64, 4);
		}
	}

	setRGBA(0, 0, 0, 0.65f);
	fillRect(0, 0, devSize.width, devSize.height, 0);
	setRGBA(1, 1, 1, 1);

	if( texLoading )
	{
		setAlpha(alpha);
		drawImage(texLoading, devSize.width - 10, devSize.height - 10, BOTTOM|RIGHT);
	}

	setAlpha(_alpha);
}
