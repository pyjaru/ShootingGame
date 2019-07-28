#pragma once

#include "iPlatform.h"

#include "iGraphics.h"
#include "iType.h"
#include "iUtil.h"

#include "iStrTex.h"
#include "iImage.h"
#include "iPopup.h"
#include "iString.h"
#include "iShortPath.h"

#include "iOpenal.h"
#include "iOpengl.h"

#include "iMatrix.h"

#if 0// $$$
#include "iSpine.h"
#endif// $$$

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

#include <math.h>
#include <time.h>

//#define DEV_WIDTH 900					// 게임마다 개발자가 설정하는 사이즈
//#define DEV_HEIGHT 1600
#define DEV_WIDTH 1600					// 게임마다 개발자가 설정하는 사이즈
#define DEV_HEIGHT 900

extern iSize monitorSize;	// 모니터 사이즈
extern iSize wndSize;		// 윈도우 창 사이즈
extern iSize devSize;		// 개발 사이즈
extern iRect viewport;		// 윈도우 창에서 사용되는 영역

typedef enum _iKeyState {
	iKeyStateBegan = 0,
	iKeyStateMoved,
	iKeyStateEnded,
	iKeyStateCancel
} iKeyState;

#define TOP 	0
#define VCENTER 1
#define BOTTOM	2
#define LEFT	4
#define HCENTER	8
#define RIGHT	16

#define REVERSE_NONE 0
#define REVERSE_WIDTH 1
#define REVERSE_HEIGHT 2

#define iButtonStateNum 4
typedef enum _iButtonState {
	iButtonStateIdle = 0,
	iButtonStateClick,
	iButtonStateHover,
	iButtonStateDisabled,
} iButtonState;

void loadLib(iSize devSize);
void freeLib();
typedef void(*DRAW_METHOD)(float);
void drawLib(void* hdc, DRAW_METHOD method);
void keyLib(iKeyState stat, int c);

#if SUPPORT_OPENGL_3
void loadTexForGL3();
void freeTexForGL3();
#endif

typedef enum _keyboard_key {
	keyboard_left       =   1, // 1 << 0
	keyboard_right      =   2, // 1 << 1
	keyboard_up         =   4, // 1 << 2
	keyboard_down       =   8, // 1 << 3
	keyboard_return     =  16, // 1 << 4
	keyboard_space      =  32, // 1 << 5
	keyboard_esc        =  64,
	keyboard_ctrl       = 128,
	keyboard_angle_up   = 256,
	keyboard_angle_down = 512,
} keyboard_key;

uint32 getKeyDown();
uint32 getKeyStat();

void setRGBA(float r, float g, float b, float a);
void getRGBA(float& r, float& g, float& b, float& a);
void setAlpha(float a);
float getAlpha();
void setRGB(float r, float g, float b);
void getRGB(float& r, float& g, float& b);

void setDotSize(float size);
void drawDot(int x, int y);
void drawDot(iPoint p);
void drawDots(iPoint* p, int pNum);

void setLineWidth(float width);
void setLineSmooth(bool enable);
void drawLine(iPoint sp, iPoint ep);
void drawLine(int sx, int sy, int ex, int ey);// start, end

void drawRect(iRect rect);
void drawRect(int x, int y, int width, int height, int radius = 0);
void fillRect(iRect rect);
void fillRect(int x, int y, int width, int height, int radius = 0);
void drawPoly(iPoint* p, int pNum, bool fill);

void setClip(float x, float y, float dx, float dy);

Texture* createImage(const char* szFormat, ...);
Texture* createImageGray(const char* szFormat, ...);
Texture* createImageBlur(int radius, const char* szFormat, ...);
Texture* createImageReflect(float rate, const char* szFormat, ...);
void freeImage(Texture* tex);
void drawImage(Texture* tex,
	float x, float y,
	float ix, float iy, float idx, float idy,// src
	float rateX, float rateY,
	int xyz, int degree,
	int anc, int reverse = 0);
void drawImage(Texture* tex, int x, int y, int anc);
void drawImage(Texture* tex, int x, int y, int width, int height, int anc);

void setStringName(const char* fontName);
const char* getStringName();
void setStringSize(float fontPoint);
float getStringSize();
void setStringRGBA(float r, float g, float b, float a);
void getStringRGBA(float& r, float& g, float& b, float& a);
void setStringBorder(float border);
float getStringBorder();
void setStringBorderRGBA(float r, float g, float b, float a);
void getStringBorderRGBA(float& r, float& g, float& b, float& a);
//void drawString(int x, int y, int anc, const char* szFormat, ...);
iSize sizeOfString(const char* szFormat, ...);
iRect rectOfString(const char* szFormat, ...);

#if (_OS_==OS_WINDOWS)
void randomize();
int random();

wchar_t* utf8_to_utf16(const char* szFormat, ...);
char* utf16_to_utf8(const wchar_t* str);

void* getDump(int dumpSize);
#define MakeArray(type, name, count)	type* name = (type*)getDump(sizeof(type)*count)
#else// gcc
#define	MakeArray(type, name, count)	type name[count]
#endif

float _sin(float degree);
float _cos(float degree);

float linear(float rate, float start, float end);
float easeIn(float rate, float start, float end);
float easeOut(float rate, float start, float end);
float easeInOut(float rate, float start, float end);



