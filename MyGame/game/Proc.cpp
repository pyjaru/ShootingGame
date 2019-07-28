#include "Proc.h"

#include "Loading.h"
#include "Menu.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Common.h"
#include "Map.h"

void generateEnemy(bool bGenEnemy);
void addScore(int point);

int score;
float procStartTime;
bool bGenEnemy = false;

void updateCursorVisible(bool visible);

void loadProc()
{
	updateCursorVisible(false);

	loadMap();
	loadBullet();
	loadEnemy(addScore);

	loadPopProc();
	loadPopSetting(showPopPause);
	loadPopPause();
	loadPopResult();
	showPopProc(true);

	audioPlay(snd_inGame);

	score = 0;
	procStartTime = 0.0f;

	player->ready();

	
}

void freeProc()
{
	updateCursorVisible(true);

	freeMap();
	freeBullet();
	freeEnemy();

	freePopSetting();
	freePopPause();
	freePopProc();
	freePopResult();
	
	audioStop();
}

void drawProc(float dt)
{
	float _dt = dt;
	if( popPause->bShow || isSettingShow() || popResult->bShow)
		dt = 0.0f;

	if(procStartTime < 3.0f)
	{
		float x = linear(procStartTime/3.0f, -devSize.width/2, devSize.width*3/8);
		player->position = iPointMake(x, devSize.height/2);
		
		audioPitch(snd_inGame, procStartTime);
		procStartTime += dt;
		dt = 0.0f;
		if (procStartTime >= 3.0f)
		{
			player->img->intervalDt = 0.1f;
			audioPlay(snd_inGame);
			audioPitch(snd_inGame, 1.0f);
			bGenEnemy = true;
		}
	}

	drawMap(dt);
	
	drawPlayer(dt, iPointZero);
	drawBullet(dt, iPointZero);
	drawEnemy(dt, iPointZero);

	drawPopProc(dt);
	drawPopPause(_dt);
	drawPopSetting(_dt);
	drawPopResult(_dt);

	generateEnemy(bGenEnemy);
}

void keyProc(iKeyState stat, iPoint point)
{
	if( keyPopPause(stat, point) || keyPopSetting(stat, point) || keyPopResult(stat, point))
		return;

	switch( stat )
	{
		case iKeyStateBegan: break;
		case iKeyStateMoved: break;
		case iKeyStateEnded: break;
		default:break;
	}
}

void addScore(int point)
{
	score += point;
}

int num = -1;
void generateEnemy(bool bGenEnemy)
{
	if (!bGenEnemy)
		return;

	if (num != numEnemy)
	{
		num = numEnemy;
	}

	if (numEnemy < 1)
	{
#if 0
		for (int i = 0; i < 9; i++)
		{
			addEnemy(iPointMake(devSize.width + 100, 80 + 80 * i));
		}
#endif
		int w = devSize.width;
		for (int i = 0; i < 16; i++)
		{
			addEnemy(iPointMake(devSize.width + 100 + random()%(w-100), 80 + random()%640 ));
		}
	}
}

///////////////////////////////////////////
// PopProc
///////////////////////////////////////////

// life, score

iPopup* popProc;
iImage** imgLives;
Texture* texLife;
iStrTex* stProcScore;
iStrTex* stProcGuide;
iStrTex* stPlayTime;
float scaleLifeImg;

float resultDelayTime;

float playTime;
void loadPopProc()
{
	Texture* tex;
	iStrTex* st;
	iImage* img;
	iSize size;

	iPopup* pop = new iPopup(iPopupStyleNone); // DevSize
	imgLives = (iImage**)malloc(sizeof(iImage*)*_playerDefaultLife);
	
	tex = createImage("assets/textures/ui/life.png");
	scaleLifeImg = 50.0f / tex->width;
	for (int i = 0; i < _playerDefaultLife; i++)
	{
		img = new iImage();
		img->addObject(tex);
		img->position = iPointMake(20 + 60 * i, 50);
		imgLives[i] = img;
	}
	freeImage(tex);

	stProcScore = new iStrTex(NULL);
	stProcGuide = new iStrTex(NULL);
	stPlayTime = new iStrTex(NULL);

	pop->setup(iPointZero, iPointZero);
	pop->show(true);
	popProc = pop;

	resultDelayTime = 0.0f;
	playTime = 0.0f;
}

void freePopProc()
{
	delete stProcScore;
	delete stProcGuide;
	delete stPlayTime;

	free(imgLives);

	delete popProc;
}

void drawPopProc(float dt)
{
	setStringSize(20.f);
	setStringBorder(2.0f);
	setStringRGBA(0, 0, 0, 1);
	setStringBorderRGBA(0.5, 0.5, 0.5, 1);
	stProcGuide->paint(devSize.width - 10, devSize.height - 10, BOTTOM | RIGHT, "이동 : 방향키, 발사: 스페이스, ESC : 일시정지");

	setStringSize(40.f);
	setStringBorder(3.0f);
	setStringRGBA(0, 0, 0, 1);
	setStringBorderRGBA(1, 1, 1, 1);
	stProcScore->paint(devSize.width/2, 50, VCENTER | HCENTER, "SCORE : %d", score);

	char strTime[100];
	playTime += dt;

	int pt = playTime;
	int sec = pt % 60;
	int min = (pt / 60) % 60;
	int hour = (pt / 3600) % 24;

	if (hour)
		sprintf(strTime, "%d시간 %d분 %d초", hour, min, sec);
	else if (min)
		sprintf(strTime, "%d분 %d초", min, sec);
	else
		sprintf(strTime, "%d초", sec);

	stPlayTime->paint(devSize.width / 2, 120, VCENTER | HCENTER, strTime);

	for (int i = 0; i < player->hp; i++)
		imgLives[i]->paint(dt, iPointZero, scaleLifeImg);

	popProc->paint(dt);

	if (player->hp == 0 && popResult->bShow == false )
	{
		resultDelayTime += dt;
		if (resultDelayTime > 1.0f)
		{
			renewScore(score);
			showPopResult(true);
		}
	}
}

bool keyPopProc(iKeyState stat, iPoint point)
{
	return false;
}

void showPopProc(bool show)
{
	popProc->show(show);
}

///////////////////////////////////////////
// PopResult
///////////////////////////////////////////

#define num_pop_result_btn 2

iPopup* popResult;
iImage** imgResultBtn;

void loadPopResult()
{
	iPopup* pop;
	Texture* tex;
	iStrTex* st;
	iImage* img;
	igImage* igImg;
	iSize size, popSize;
	iGraphics* g;

	pop = new iPopup(iPopupStyleMove);

	// GameOver - 팝업 배경
	g = iGraphics::share();
	popSize = iSizeMake(devSize.width/4, devSize.height/4);
	g->init(popSize);
	igImg = g->createIgImage("assets/textures/ui/green_panel.png");
	g->drawIgImage(igImg, 0, 0, popSize.width, popSize.height, TOP | LEFT);
	g->freeIgImage(igImg);
	
	// GameOver - 팝업 제목 라벨
	igImg = g->createIgImage("assets/textures/ui/yellow_button13.png");
	g->drawIgImage(igImg, popSize.width/2, popSize.height/5-10, TOP | HCENTER);
	g->freeIgImage(igImg);

	setStringSize(30);
	setStringBorder(2);
	setStringRGBA(0, 0, 0, 1);
	setStringBorderRGBA(1, 1, 1, 1);
	size = g->sizeOfString("Game Over!");

	g->drawString(popSize.width/2, popSize.height/5, TOP | HCENTER, "Game Over!");

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	pop->addObject(img);

	// 다시 시작, 게임 종료
	const char* strBtn[num_pop_result_btn] = { "나가기", "한 번 더!" };
	imgResultBtn = (iImage**)malloc(sizeof(iImage*) * num_pop_result_btn);

	size = iSizeMake(popSize.width/5*2, popSize.height/5);
	for (int btnIdx = 0; btnIdx < num_pop_result_btn; btnIdx++)
	{
		img = new iImage();
		for (int btnStateIdx = 0; btnStateIdx < iButtonStateNum; btnStateIdx++)
		{
			g->init(size);

			if (btnStateIdx == iButtonStateIdle || btnStateIdx == iButtonStateDisabled)
				igImg = g->createIgImage("assets/textures/ui/red_button%02d.png", 0);
			else
				igImg = g->createIgImage("assets/textures/ui/red_button%02d.png", btnStateIdx);

			g->drawIgImage(igImg, 0, 0, size.width, size.height, TOP | LEFT);
			g->freeIgImage(igImg);

			setStringRGBA(1, 1, 1, 1);
			setStringSize(28);
			g->drawString(size.width / 2, size.height / 2, VCENTER | HCENTER, strBtn[btnIdx]);

			if (btnStateIdx == iButtonStateDisabled)
				tex = g->getTextureWithGrayscale();
			else
				tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->anc = VCENTER | HCENTER;
		img->position = iPointMake(popSize.width/4*(1+2*btnIdx) , popSize.height/3*2);
		pop->addObject(img);
		imgResultBtn[btnIdx] = img;
	}

	pop->setup(iPointMake(devSize.width/2-popSize.width/2, -600), 
		       iPointMake(devSize.width/2-popSize.width/2, devSize.height/2 - popSize.height/2));
	popResult = pop;
}

void freePopResult()
{
	delete popResult;
	free(imgResultBtn);
}

void drawPopResult(float dt)
{
	popResult->paint(dt);
}

void methodPopResultPaintBefore(void* parm, iPoint point, float scale, float alpha, float dt)
{
	int btnImgIdx;
	for (int btnIdx = 0; btnIdx < num_pop_result_btn; btnIdx++)
	{
		btnImgIdx = (popResult->selected == btnIdx) ? iButtonStateHover : iButtonStateIdle;
		imgResultBtn[btnIdx]->setTexAtIndex(btnImgIdx);
	}
}


bool keyPopResult(iKeyState stat, iPoint point)
{
	if (popResult->bShow == false)
		return false;
	if (popResult->stat != iPopupStatProc)
		return true;

	int i, j = -1;

	switch (stat)
	{
		case iKeyStateBegan:
		{
			if (popResult->selected == 0)
				setLoading(gs_menu, freeProc, loadMenu);
			else
				setLoading(gs_proc, freeProc, loadProc);
			
			break;
		}
		case iKeyStateMoved:
		{
			for (i = 0; i < num_pop_result_btn; i++)
			{
				if (containPoint(point, imgResultBtn[i]->touchRect(popResult->closePoint)))
				{
					j = i;
					break;
				}
			}
			if (popResult->selected != j)
			{
				audioPlay(snd_btnSwitch);
				popResult->selected = j;
			}
			break;
		}
	}

	return true;
}

void showPopResult(bool show)
{
	popResult->show(show);
	updateCursorVisible(show);
}


///////////////////////////////////////
// popPause - "돌아가기", "게임 설정", "메인 메뉴로"
///////////////////////////////////////

#define num_pop_pause_btn 3
#define close_pause_open_game     0
#define close_pause_open_settings 1
#define close_pause_open_menu     2

void methodPopPausePaintBefore(void* parm, iPoint point, float scale, float alpha, float dt);
void methodPopPauseClose(void* parm);

iPopup* popPause;
iImage** imgPauseBtn;
int closePopPauseOpen;

void loadPopPause()
{
	const char* strProcMenu[num_pop_pause_btn] = {
		"돌아가기", "게임 설정",  "메인 메뉴로"
	};

	int btnIdx, btnStateIdx;
	Texture* tex;
	igImage* igImg;
	iImage* img;
	iSize popSize = iSizeMake(400, 400);
	iSize btnSize = iSizeMake(300, 80);
	iPopup* pop = new iPopup(iPopupStyleZoom);

	iGraphics* g = iGraphics::share();
	g->init(popSize);
	igImg = g->createIgImage("assets/textures/ui/blue_panel.png");
	g->drawIgImage(igImg, 0, 0, popSize.width, popSize.height, TOP | LEFT);
	g->freeIgImage(igImg);
	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	imgPauseBtn = (iImage**)malloc(sizeof(iImage)*4);
	for(btnIdx = 0 ; btnIdx < num_pop_pause_btn ; btnIdx++)
	{
		img = new iImage();
		for(btnStateIdx = 0 ; btnStateIdx < iButtonStateNum ; btnStateIdx++)
		{
			g->init(btnSize);

			setRGBA(1, 1, 1, 1);
			setStringRGBA(0, 0, 0, 1);
			setStringSize(20.0f);
			
			if (btnStateIdx == iButtonStateIdle || btnStateIdx == iButtonStateDisabled)
				igImg = g->createIgImage("assets/textures/ui/red_button%02d.png", 7);
			else
				igImg = g->createIgImage("assets/textures/ui/red_button%02d.png", 7 + btnStateIdx);

			g->drawIgImage(igImg, 0, 0, btnSize.width, btnSize.height, TOP | LEFT);
			g->freeIgImage(igImg);
			g->drawString(btnSize.width/2, btnSize.height/2, VCENTER|HCENTER, strProcMenu[btnIdx]);

			if (btnStateIdx == iButtonStateDisabled)
				tex = g->getTextureWithGrayscale();
			else
				tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->anc =  HCENTER|VCENTER;
		img->position = iPointMake(popSize.width/2, 100 + 100*btnIdx);
		pop->addObject(img);
		imgPauseBtn[btnIdx] = img;
	}
	pop->methodClose = methodPopPauseClose;
	pop->methodPaintBefore = methodPopPausePaintBefore;
	pop->setup(iPointMake(devSize.width/2, devSize.height/2),
			   iPointMake(devSize.width/2 - popSize.width/2,
					      devSize.height/2 - popSize.height/2));

	popPause = pop;

}

void freePopPause()
{
	delete imgPauseBtn;
	delete popPause;
}

void drawPopPause(float dt)
{
	popPause->paint(dt);

	if( getKeyStat()&keyboard_esc)
	{
		closePopPauseOpen = -1;
		popPause->selected = -1;

		if (popPause->bShow == false)
		{
			showPopPause(true);
		}
		else if (popPause->stat == iPopupStatProc)
		{
			closePopPauseOpen = close_pause_open_game;
			showPopPause(false);
		}
	}
}

void methodPopPausePaintBefore(void* parm, iPoint point, float scale, float alpha, float dt)
{
	int btnImgIdx;
	for(int btnIdx = 0 ; btnIdx < num_pop_pause_btn ; btnIdx++)
	{
		btnImgIdx = (popPause->selected == btnIdx) ? iButtonStateHover : iButtonStateIdle;
		imgPauseBtn[btnIdx]->setTexAtIndex(btnImgIdx);
	}
}

void methodPopPauseClose(void* parm)
{
	switch( closePopPauseOpen )
	{
		case close_pause_open_game:
		{
			break;
		}
		case close_pause_open_settings:
		{
			updateCursorVisible(true);
			showPopSetting(true);
			break;
		}
		case close_pause_open_menu:
		{
			setLoading(gs_menu, freeProc, loadMenu);
			break;
		}
		default: break;
	}
}

bool keyPopPause(iKeyState stat, iPoint point)
{
	if( popPause->stat != iPopupStatProc )
		return false;

	switch( stat )
	{
		case iKeyStateBegan:
		{
			if(popPause->selected == -1)
				break;

			closePopPauseOpen = popPause->selected;
			showPopPause(false);
			break;
		}
		case iKeyStateMoved:
		{
			int btnIdx, prevBtnIdx = -1;
			for(btnIdx = 0 ; btnIdx < num_pop_pause_btn ; btnIdx++)
			{
				if(containPoint(point, imgPauseBtn[btnIdx]->touchRect(popPause->closePoint)))
				{
					prevBtnIdx = btnIdx;
					break;
				}
			}
			popPause->selected = prevBtnIdx;
			break;
		}
		case iKeyStateEnded:
		{
			break;
		}
		default:break;
	}

	return true;
}

void showPopPause(bool show)
{
	popPause->show(show);
	updateCursorVisible(show);
}