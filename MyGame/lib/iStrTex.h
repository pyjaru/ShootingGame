#pragma once

#include "iGraphics.h"

typedef Texture* (*MethodStrTex)(const char*);

class iStrTex
{
public:
	iStrTex(MethodStrTex method=NULL);
	virtual ~iStrTex();

public:
	void setString(const char* szFormat, ...);
	void paint(float x, float y, int anc, const char* szFormat, ...);

public:
	char* str;
	Texture* tex;
	MethodStrTex method;
};
