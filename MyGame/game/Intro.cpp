#include "Intro.h"

#include "Loading.h"
#include "Menu.h"
#include "Common.h"

///////////////////////////////////////
// Intro
///////////////////////////////////////

#define _introDt  3.0f

static bool once = true;
static float introDt = 0.0f;
Texture** texIntro;

void loadIntro()
{
	const char *str[2] = {
		"BCA Programming 14th 최용민",	"Touch to continue..."
	};

	texIntro = (Texture**)malloc(sizeof(Texture*) * 2);

	Texture* tex;
	iSize size;
	iGraphics* g = iGraphics::share();

	size = sizeOfString(str[0]);
	{
		g->init(size);

		setStringSize(20);
		setStringName("assets/fonts/D2Coding-Ver1.3.2.ttf");
		setStringRGBA(1, 1, 1, 1);
		g->drawString(0, 0, TOP | LEFT, str[0]);

		texIntro[0] = g->getTexture();
	}
	size = sizeOfString(str[1]);
	{
		g->init(size);

		setStringSize(20);
		setStringRGBA(1, 1, 1, 1);
		g->drawString(0, 0, TOP | LEFT, str[1]);

		texIntro[1] = g->getTexture();
	}

	introDt = 0.0f;
}

void freeIntro()
{
	for (int i = 0; i < 2; i++)
		freeImage(texIntro[i]);
	
	free(texIntro);
}

void drawIntro(float dt)
{
	setRGBA(0, 0, 0, 1);
	fillRect(iRectMake(0, 0, devSize.width, devSize.height));
	setRGBA(1, 1, 1, 1);

	drawImage(texIntro[0], 64, devSize.height - 64,
			  0, 0, texIntro[0]->width, texIntro[0]->height,
			  1, 1,
			  2, 0,
			  BOTTOM|LEFT, 0);

	float _alpha = getAlpha();
	float alpha = _sin(180*introDt/2.0f);
	setAlpha(alpha);
	drawImage(texIntro[1], devSize.width/2, devSize.height*2/3,
				  0, 0, texIntro[1]->width, texIntro[1]->height,
				  1, 1,
				  2, 0,
				  VCENTER|HCENTER, 0);
	setAlpha(_alpha);

	introDt += dt;
	if( introDt > 2.0f )
		introDt -= 2.0f;
}

void keyIntro(iKeyState stat, iPoint point)
{
	if(once && stat == iKeyStateBegan)
	{
		once = false;
		audioPlay(snd_introClick);
		setLoading(gs_menu, freeIntro, loadMenu);
	}
}
