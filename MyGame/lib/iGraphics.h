#pragma once

#include "iPlatform.h"

#if (_OS_==OS_WINDOWS)

#include <Unknwn.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")

#include "iType.h"

typedef struct _Texture {
	uint32 texID;
	int width, height;
	int potWidth, potHeight;
	int retainCount;// 참조된 개수
} Texture;

using namespace Gdiplus;

typedef void igImage;// GdiPlus::Image or (iOS/Mac CGImage) or (Android Bitmap)

class iGraphics
{
private:
	iGraphics();
public:
	~iGraphics();

public:
	static iGraphics* share();
	void init(iSize size);
	Texture* getTexture();
	Texture* getTextureWithGrayscale();

	void initGraphics();
	//void drawDot(int x, int y);
	void drawLine(int sx, int sy, int ex, int ey);// start, end
	void drawRect(int x, int y, int width, int height, int radius);
	void fillRect(int x, int y, int width, int height, int radius);

	static igImage* createIgImage(const char* szFormat, ...);
	static void freeIgImage(igImage*);
	static int getIgImageWidth(igImage* img);
	static int getIgImageHeight(igImage* img);
	void drawIgImage(igImage* img,
		int x, int y,
		int ix, int iy, int idx, int idy,// src
		float rateX, float rateY,
		int xyz, int degree,
		int anc);
	void drawIgImage(igImage* img, int x, int y, int anc);
	void drawIgImage(igImage* img, int x, int y, int width, int height, int anc);

	static uint8* bmp2rgba(Bitmap* bmp, uint32& width, uint32& height);
	static uint8* bmp2rgbaGrayscale(Bitmap* bmp, uint32& width, uint32& height);

	void drawString(int x, int y, int anc, const char* szFormat, ...);
	iSize sizeOfString(const char* szFormat, ...);

public:
	Bitmap* bitmap;
	Graphics* g;

private:
};

uint8 float2byte(float f);

Texture* createImageWidthRGBA(uint8* rgba, float width, float height);

void checkFontFamily(FontFamily* ff, int& fontStyle);

#endif

