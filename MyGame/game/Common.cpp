#include "Common.h"

void drawPopCommon(iPopup* pop, float alpha)
{
	float a;
	if( pop->stat==iPopupStatOpen)
	{
		a = pop->aniDt/pop->_aniDt;// 0, 1
		a *= 0.5f;
	}
	else if( pop->stat==iPopupStatProc)
	{
		a = 0.5f;
	}
	else// if( pop->stat==iPopupStatClose)
	{
		a = 1 - pop->aniDt/pop->_aniDt;// 0, 1
	}
	setRGBA(0, 0, 0, a);
	fillRect(0, 0, devSize.width, devSize.height);
	setRGBA(1, 1, 1, 1);
}


///////////////////////////////////////
// PopSettings
///////////////////////////////////////

iPopup* popSetting;
iImage** imgSettingBtn;

iPoint prevPosition;
int movingBtn;

#define num_setting_btn 3
#define option_min 30// 50-20
#define option_max 330// 350-20

void closePopSetting(void* parm);
void drawPopSettingBefore(void* parm, iPoint point, float scale, float alpha, float dt);
CLOSE_SETTING_METHOD methodCloseSetting;

void loadPopSetting(CLOSE_SETTING_METHOD method)
{
	igImage* igImg;
	iImage* img;
	Texture* tex;
	int i;

	iPopup* pop = new iPopup(iPopupStyleNone);

	iGraphics* g = iGraphics::share();
	iSize popSize = iSizeMake(400, 400);
	iSize size;

	setRGBA(1, 1, 1, 1);
	g->init(popSize);

	igImg = g->createIgImage("assets/textures/ui/yellow_panel.png");
	g->drawIgImage(igImg, 0, 0, popSize.width, popSize.height, TOP | LEFT);
	g->freeIgImage(igImg);

	const char* strBg[2] = { "BGM", "SFX" };
	setStringSize(30);
	setStringRGBA(0, 0, 0, 1);
	setStringBorder(3);
	setStringBorderRGBA(0.7f, 0.7f, 0.7f, 1.0f);

	setRGBA(0, 0, 0, 1);
	for (i = 0; i < 2; i++)
	{
		g->drawString(50, 50 + 200 * i, VCENTER | LEFT, strBg[i]);
		g->fillRect(50, 150 + 200 * i, 300, 4, 2);
	}

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	pop->addObject(img);

	imgSettingBtn = (iImage**)malloc(sizeof(iImage*) * 3);
	// x
	size = iSizeMake(64, 64);
	img = new iImage();
	for (i = 0; i < 2; i++)
	{
		g->init(size);

		if (i == 0)
			igImg = g->createIgImage("assets/textures/ui/grey_boxCross.png");
		else// if( i==1 )
			igImg = g->createIgImage("assets/textures/ui/yellow_boxCross.png");

		g->drawIgImage(igImg, 0, 0, size.width, size.height, TOP | LEFT);
		g->freeIgImage(igImg);

		tex = g->getTexture();
		img->addObject(tex);

		freeImage(tex);
	}
	img->position = iPointMake(popSize.width - size.width, 0);
	pop->addObject(img);
	imgSettingBtn[0] = img;

	// Handle x 2 (40x40)
	size = iSizeMake(40, 40);
	g->init(size);

	setRGBA(0, 0, 0, 1);
	g->fillRect(0, 0, size.width, size.height, 20);

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);
	img->position = iPointMake(50 - size.width / 2, 150 - size.width / 2);
	pop->addObject(img);
	imgSettingBtn[1] = img;

	img = img->copy();
	img->position.y += 200;
	pop->addObject(img);
	imgSettingBtn[2] = img;

	pop->setup(iPointMake(devSize.width / 2 - 200, devSize.height / 2 - 200),
		iPointMake(devSize.width / 2 - 200, devSize.height / 2 - 200));
	pop->methodClose = closePopSetting;
	pop->methodPaintBefore = drawPopSettingBefore;
	popSetting = pop;

	movingBtn = 0;

	imgSettingBtn[1]->position.x = option_min + (option_max - option_min)*appData->bgm;
	imgSettingBtn[2]->position.x = option_min + (option_max - option_min)*appData->eff;

	methodCloseSetting = method;
}

void freePopSetting()
{
	delete popSetting;
	free(imgSettingBtn);
}

void showPopSetting(bool show)
{
	popSetting->show(show);
}

void closePopSetting(void* parm)
{
	if(methodCloseSetting)
		methodCloseSetting(true);
}

void drawPopSetting(float dt)
{
	popSetting->paint(dt);
}

void drawPopSettingBefore(void* parm, iPoint point, float scale, float alpha, float dt)
{
	// 닫기 버튼 마우스 Hover
	int btnImgIdx = (popSetting->selected == 0);
	imgSettingBtn[0]->setTexAtIndex(btnImgIdx);
}

bool keyPopSetting(iKeyState stat, iPoint point)
{
	if (popSetting->stat != iPopupStatProc)
		return false;

	int btnIdx, prevBtnIdx = -1;

	switch (stat) {

	case iKeyStateBegan:
		btnIdx = popSetting->selected;
		if (btnIdx == -1) break;

		if (btnIdx == 0) // Button Close
		{
			saveAppData();
			showPopSetting(false);
		}
		else // Volume Handle
		{
			prevPosition = point;
			movingBtn = btnIdx;
		}
		break;

	case iKeyStateMoved:
		if (movingBtn == 0)
		{
			for (btnIdx = 0; btnIdx < num_setting_btn; btnIdx++)
			{
				if (containPoint(point, imgSettingBtn[btnIdx]->touchRect(popSetting->closePoint)))
				{
					prevBtnIdx = btnIdx;
					break;
				}
			}
			if (popSetting->selected != prevBtnIdx)
				audioPlay(snd_btnSwitch);
			popSetting->selected = prevBtnIdx;
		}
		else// if( movingBtn==1 || movingBtn==2 )
		{
			iPoint dp = point - prevPosition;
			prevPosition = point;

			iImage* img = imgSettingBtn[movingBtn];
			img->position.x += dp.x;
			// min max
			if (img->position.x < option_min)
				img->position.x = option_min;
			else if (img->position.x > option_max)
				img->position.x = option_max;
		}
		break;

	case iKeyStateEnded:
		if (movingBtn > 0)
		{
			float x = imgSettingBtn[movingBtn]->position.x;
			float snd = (x - option_min) / (option_max - option_min);
			if (movingBtn == 1)		appData->bgm = snd;
			else					appData->eff = snd;
			audioVolume(appData->bgm, appData->eff, snd_mainMenu);

			movingBtn = 0;
		}
		break;

	default:
		break;
	}

	return true;
}

bool isSettingShow()
{
	return popSetting->bShow;
}

//////////////////////////////////////////////////////
// PopExit
//////////////////////////////////////////////////////

#define num_pop_exit_btn 2

iPopup* popExit = NULL;
iImage** imgExitBtn;

void drawPopExitBefore(void* parm, iPoint point, float scale, float alpha, float dt);

void createPopExit()
{
	igImage* igImg;
	iImage* img;
	Texture* tex;
	int btnIdx, btnStateIdx;

	iPopup* pop = new iPopup(iPopupStyleZoom);
	
	iGraphics* g = iGraphics::share();
	iSize popSize = iSizeMake(300, 400);
	iSize btnSize = iSizeMake(80, 60);

	setRGBA(1, 1, 1, 1);
	g->init(popSize);
	igImg = g->createIgImage("assets/textures/ui/red_panel.png");
	g->drawIgImage(igImg, 0, 0, popSize.width, popSize.height, TOP | LEFT);
	g->freeIgImage(igImg);
	
	igImg = g->createIgImage("assets/textures/ui/yellow_button13.png");
	g->drawIgImage(igImg, popSize.width/2, popSize.height/5, VCENTER|HCENTER);
	g->freeIgImage(igImg);

	setStringSize(34);
	setStringBorder(2);
	setStringRGBA(0, 0, 0, 1);
	setStringBorderRGBA(1, 1, 1, 1);
	g->drawString(popSize.width/2, popSize.height/5, VCENTER | HCENTER, "게임 종료");

	setStringSize(28);
	setStringBorder(0);
	setStringRGBA(1, 1, 1, 1);
	setStringBorderRGBA(1, 1, 1, 1);
	g->drawString(popSize.width/2, popSize.height/2, VCENTER|HCENTER, "종료하겠습니까?");
	
	setRGBA(1, 1, 1, 1);

	tex = g->getTexture();
	img = new iImage();
	img->addObject(tex);
	freeImage(tex);

	pop->addObject(img);

	// iImage 버튼 이미지 - 아니오 / 예
	imgExitBtn = (iImage**)malloc(sizeof(iImage*)*2);
	const char* strBtn[2] = {"아니오", "예"};

	for(btnIdx=0; btnIdx < num_pop_exit_btn; btnIdx++)
	{
		img = new iImage();
		for(btnStateIdx=0; btnStateIdx< iButtonStateNum; btnStateIdx++)
		{
			g->init(btnSize);
			
			if (btnStateIdx == iButtonStateIdle || btnStateIdx == iButtonStateDisabled)
				igImg = g->createIgImage("assets/textures/ui/blue_button%02d.png", 7);
			else
				igImg = g->createIgImage("assets/textures/ui/blue_button%02d.png", 7 + btnStateIdx);

			g->drawIgImage(igImg, btnSize.width/2, btnSize.height*2/10, btnSize.width, btnSize.height, VCENTER|HCENTER);
			g->freeIgImage(igImg);

			setStringSize(20);
			setStringBorder(2);
			setStringRGBA(1, 1, 1, 1);
			setStringBorderRGBA(0, 0, 0, 1);
			g->drawString(btnSize.width/2, btnSize.height*4/10 - 5, VCENTER|HCENTER, strBtn[btnIdx]);

			if (btnStateIdx == iButtonStateDisabled)
				tex = g->getTextureWithGrayscale();
			else
				tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
		}
		img->position = iPointMake((popSize.width/5 * (1+3*btnIdx)) - btnSize.width/2, popSize.height*8/10);
		pop->addObject(img);
		imgExitBtn[btnIdx] = img;
	}

	pop->methodPaintBefore = drawPopExitBefore;
	pop->setup( iPointMake(devSize.width/2, devSize.height/2),
				iPointMake(devSize.width/2-200, devSize.height/2-200));
	popExit = pop;
}

void freePopExit()
{
	if( popExit==NULL )
		return;

	delete popExit;
	free( imgExitBtn );
}

void showPopExit(bool show)
{
	if( popExit==NULL )
		createPopExit();

	popExit->show(show);
}

void drawPopExitBefore(void* parm, iPoint position, float scale, float alpha, float dt)
{
	drawPopCommon(popExit, alpha);

	int btnImgIdx;
	for (int btnIdx = 0; btnIdx < num_pop_exit_btn; btnIdx++)
	{
		btnImgIdx = (popExit->selected == btnIdx) ? iButtonStateHover : iButtonStateIdle;
		imgExitBtn[btnIdx]->setTexAtIndex(btnImgIdx);
	}
}

void drawPopExit(float dt)
{
	if( popExit==NULL )
		return;

	popExit->paint(dt);
}


bool keyPopExit(iKeyState stat, iPoint point)
{
	if( popExit==NULL || popExit->stat!=iPopupStatProc )
		return false;

	int btnIdx, prevBtnIdx = -1;

	switch( stat ) {

	case iKeyStateBegan:
		if( popExit->selected == 0)
		{
			// 종료 취소
			showPopExit(false);
			
		}
		else
		{
			// 종료 승인
			void exitProgram();
			exitProgram();
		}
		break;

	case iKeyStateMoved:
		for(btnIdx=0; btnIdx<2; btnIdx++)
		{
			if( containPoint(point, imgExitBtn[btnIdx]->touchRect(popExit->closePoint)) )
			{
				if (imgExitBtn[btnIdx]->imgIndex == iButtonStateDisabled)
					continue;

				prevBtnIdx = btnIdx;
				break;
			}
		}
		popExit->selected = prevBtnIdx;
		break;

	case iKeyStateEnded:
		break;

	default:
		break;
	}

	return true;
}

//////////////////////////////////////////////////////
// Font
//////////////////////////////////////////////////////
iFont::iFont()
{
	texs = (Texture**)calloc(sizeof(Texture*), 256);

	gap = -10;//1.0f;
}

iFont::~iFont()
{
	for(int i=0; i<256; i++)
	{
		if( texs[i] )
			freeImage( texs[i] );
	}
	free( texs );
}

iSize iFont::sizeOfString(float rate, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	char szText[1024];
	_vsnprintf(szText, sizeof(szText), szFormat, args);

	va_end(args);

	Texture* tex = NULL;
	int i, j, len = strlen(szText);
	float x = 0;
	for(i=0; i<len; i++)
	{
		j = szText[i];
		tex = texs[j];
#if 1
		if( tex==NULL ) continue;
#endif
		x += (tex->width + gap)*rate;
	}

	return iSizeMake(x, tex->height);
}

void iFont::drawString(float x, float y, int anc, float rate, const char* szFormat, ...)
{
	va_list args;
	va_start(args, szFormat);

	char szText[1024];
	_vsnprintf(szText, sizeof(szText), szFormat, args);

	va_end(args);

	iSize size = sizeOfString(rate, szText);
	switch( anc ) {
	case TOP|LEFT:													break;
	case TOP|HCENTER:		x -= size.width/2;						break;
	case TOP|RIGHT:			x -= size.width;						break;
	case VCENTER|LEFT:							y -= size.height/2;	break;
	case VCENTER|HCENTER:	x -= size.width/2;	y -= size.height/2;	break;
	case VCENTER|RIGHT:		x -= size.width;	y -= size.height/2;	break;
	case BOTTOM|LEFT:							y -= size.height;	break;
	case BOTTOM|HCENTER:	x -= size.width/2;	y -= size.height;	break;
	case BOTTOM|RIGHT:		x -= size.width;	y -= size.height;	break;
	}

	Texture* tex;
	int i, j, len = strlen(szText);
	for(i=0; i<len; i++)
	{
		j = szText[i];
		tex = texs[j];
#if 1
		if( tex==NULL ) continue;
#endif
		drawImage(tex, x, y,
					0, 0, tex->width, tex->height,
					rate, rate,
					2, 0, anc);
		x += (tex->width + gap)*rate;
	}
}


//////////////////////////////////////////////////////
// Sound
//////////////////////////////////////////////////////
void createSound()
{
	// num에 총 오디오 파일 + 1개 삽입..(사운드가 한번씩 안나오는 경우가 발생한다.)
	int i, num = snd_totalnum+1;
	MakeArray(AudioInfo, ai, num);
	for(i=0; i<snd_totalnum; i++)
	{
		ai[i].fileName = (char*)malloc(sizeof(char)*32);
		sprintf(ai[i].fileName, "assets/sounds/%d.wav", i);

		if( i<snd_mainMenu )
			ai[i].bRepeat = false;
		else
			ai[i].bRepeat = true;
		
		ai[i].volume = 1.0f;
		ai[i].pitch = 1.0f;
	}

	// 더미로 삽입..(사운드가 한번씩 안나오는 경우가 발생한다.)
	ai[snd_totalnum].fileName = (char*)malloc(sizeof(char)*32);
	sprintf(ai[snd_totalnum].fileName, "assets/sounds/%d.wav", 0);

	ai[snd_totalnum].bRepeat = false;
	ai[snd_totalnum].volume = 1.0f;
	ai[snd_totalnum].pitch = 1.0f;

	loadAudio( ai, num );

	for(i=0; i<num; i++)
		free(ai[i].fileName);
}

void freeSound()
{
	freeAudio();
}

//////////////////////////////////////////////////////
// Save 및 Stage 데이터
//////////////////////////////////////////////////////

AppData* appData;

void freeAppData()
{
	free( appData );
}

void saveAppData()
{
	writeFile(APP_FILE, (char*)appData, sizeof(AppData));
}
void loadAppData()
{
	int len;
	appData = (AppData*)loadFile(len, APP_FILE);
	if( appData==NULL )
	{
		appData = (AppData*)calloc(sizeof(AppData), 1);

		saveAppData();
	}
}

void renewScore(int score)
{
	if(appData->bestScore < score)
	{
		appData->bestScore = score;
		saveAppData();
	}
}
