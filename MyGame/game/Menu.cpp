#include "Menu.h"

#include "Common.h"
#include "Loading.h"
#include "Intro.h"
#include "Proc.h"
#include "Player.h"

///////////////////////////////////////
// Menu
///////////////////////////////////////

iStrTex* stBestScore;
Texture* texMenuBg;
iPoint movePoint;

void loadMenu()
{
	texMenuBg = createImage("assets/textures/background/background_0.jpg");
	stBestScore = new iStrTex(NULL);

	loadPopMenu();
	loadPopSetting(showPopMenu);

	showPopMenu(true);
	audioPlay(snd_mainMenu);
	
	movePoint = iPointMake(devSize.width / 2, devSize.height / 2);
}

void freeMenu()
{
	delete stBestScore;

	freeImage(texMenuBg);

	freePopMenu();
	freePopSetting();

	audioStop();
}

void drawMenu(float dt)
{
	setRGBA(1, 1, 1, 1);
	drawImage(texMenuBg, 0, 0, devSize.width, devSize.height, TOP | LEFT);

	player->paint(dt, movePoint);

	drawPopMenu(dt);
	drawPopSetting(dt);

	setStringSize(30.0f);
	setStringBorder(5.0f);
	setStringRGBA(1, 1, 1, 1);
	setStringBorderRGBA(0, 0, 0, 1);
	
	char bestScore[50];
	sprintf(bestScore, "BEST SCORE : %d", appData->bestScore);
	stBestScore->paint(50, 50, TOP | LEFT, bestScore);
}

void keyMenu(iKeyState stat, iPoint point)
{
	if (stat == iKeyStateBegan)
	{
		player->setImgReverse(point);
		movePoint = point;
	}

	if(keyPopMenu(stat, point) || keyPopSetting(stat, point))
		return;
}

///////////////////////////////////////
//   PopMenu
///////////////////////////////////////

#define num_menu_btn  3
#define close_menu_open_ready    0
#define close_menu_oepn_settings 1
#define close_menu_oepn_exit     2

int closePopMenuOpen;

iPopup* popMenu;
iImage** imgMenuBtn;

// 상점
const char* strMenu[num_menu_btn] =
{
	"게임 시작", "환경설정", "게임 종료"
};

void closePopMenu(void* parm);
void popMenuPaintBefore(void* parm, iPoint point, float scale, float alpha, float dt);

void loadPopMenu()
{
	iGraphics* g;
	igImage* igImg;
	iImage* img;
	Texture* tex;
	iSize popSize, btnSize;
	int menuIdx, btnStateIdx;

	iPopup* pop = new iPopup(iPopupStyleZoom);
	popSize = iSizeMake(800, 800);
	btnSize = iSizeMake(300, 120);

	imgMenuBtn = (iImage**)malloc(sizeof(iImage*)*num_menu_btn);

	g = iGraphics::share();
	for(menuIdx = 0 ; menuIdx < num_menu_btn ; menuIdx++)
	{
		img = new iImage();
		for(btnStateIdx = 0; btnStateIdx < iButtonStateNum ; btnStateIdx++)
		{
			setRGBA(1, 1, 1, 1);
			g->init(btnSize);

			if(btnStateIdx== iButtonStateIdle || btnStateIdx== iButtonStateDisabled)
				igImg = g->createIgImage("assets/textures/ui/blue_button00.png");
			else //if (j==1)
				igImg = g->createIgImage("assets/textures/ui/blue_button%02d.png", btnStateIdx);
					
			// Draw
			g->drawIgImage(igImg, 0, 0, btnSize.width, btnSize.height, TOP | LEFT);
			g->freeIgImage(igImg);

			setStringSize(50);
			setStringBorderRGBA(0.7f, 0.7f, 0.7f, 1);
			setStringBorder(4);
			setStringRGBA(0, 0, 0, 1);
			g->drawString(btnSize.width/2, btnSize.height/2, VCENTER|HCENTER, strMenu[menuIdx]);
			setStringRGBA(1, 1, 1, 1);
			

			if(btnStateIdx == iButtonStateDisabled)
				tex = g->getTextureWithGrayscale();
			else
				tex = g->getTexture();

			img->addObject(tex);
			freeImage(tex);
			
			setRGBA(1, 1, 1, 1);
		}
		img->position = iPointMake(0, (btnSize.height+20)*menuIdx);
		pop->addObject(img);
		imgMenuBtn[menuIdx] = img;
	}

	pop->setup(iPointMake(devSize.width + popSize.width, 100),
			   iPointMake(devSize.width/2 + popSize.width/2, 100));

	pop->methodClose = closePopMenu;
	pop->methodPaintBefore = popMenuPaintBefore;

	popMenu = pop;
}

void freePopMenu()
{
	delete imgMenuBtn;
	delete popMenu;
}

void drawPopMenu(float dt)
{
	popMenu->paint(dt);
}

void showPopMenu(bool show)
{
	popMenu->show(show);
}

bool keyPopMenu(iKeyState stat, iPoint point)
{
	if( popMenu->stat != iPopupStatProc )
		return false;

	int menuSelected = popMenu->selected;
	int menuIdx, prevMenuIdx = -1;

	switch(stat)
	{
		case iKeyStateBegan:
		{
			if( menuSelected == -1) break;
			else if(menuSelected < close_menu_oepn_exit)
			{
				if(imgMenuBtn[menuSelected]->imgIndex == iButtonStateDisabled)
					break;

				closePopMenuOpen = menuSelected;
				showPopMenu(false);
			}
			else
			{
				void showPopExit(bool show);
				showPopExit(true);
			}

			audioPlay(snd_click);
			break;
		}

		case iKeyStateMoved:
		{
			for(menuIdx = 0 ; menuIdx < num_menu_btn ; menuIdx++)
			{
				if(containPoint(point, imgMenuBtn[menuIdx]->touchRect(popMenu->closePoint)) )
				{
					if(imgMenuBtn[menuIdx]->imgIndex == iButtonStateDisabled)
						continue;
					prevMenuIdx = menuIdx;
					break;
				}
			}
			if(popMenu->selected != prevMenuIdx)
				audioPlay(snd_btnSwitch);

			popMenu->selected = prevMenuIdx;
			break;
		}

		case iKeyStateEnded:
			break;

		default:
			break;
	}
	return true;
}


void closePopMenu(void* parm)
{
	if (closePopMenuOpen == close_menu_open_ready)
		setLoading(gs_proc, freeMenu, loadProc);
	else if (closePopMenuOpen == close_menu_oepn_settings)
		showPopSetting(true);
	
}

void popMenuPaintBefore(void* parm, iPoint point, float scale, float alpha, float dt)
{
	int btnImgIdx = 0;
	for(int menuIdx = 0 ; menuIdx < num_menu_btn ; menuIdx++)
	{
		btnImgIdx = (popMenu->selected == menuIdx) ? iButtonStateHover : iButtonStateIdle;
		imgMenuBtn[menuIdx]->setTexAtIndex(btnImgIdx);
	}
}

