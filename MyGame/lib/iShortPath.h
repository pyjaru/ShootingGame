#pragma once

#include "iType.h"

typedef struct _Path {
	int value;
	bool visit;
	int* path;
	int pathNum;
} Path;

class iShortPath
{
public:
	iShortPath();
	virtual ~iShortPath();

public:
	void init(int w, int h);
	void run(int* value, int start, int end, int* path, int& pathNum);
	void run(int* value, int start, int end, iPoint* path, int& pathNum);

public:
	int w, h, wh;
	Path* path;
};

#define sp_inf 999

#define tile_width 	960/5 //20
#define tile_height	480/5 //30










#if 0
	int w = 9;
	int h = 6;
	int value[w*h] = {
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 2, 1, 1, 1, 1, 1, 1, 1,// (1, 1) => 1 + w*1 : start
		1, 1, 2, 1, 1, 1, 1, 1, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 2, 1, 1, 1, 1, 1, 1, 1,
		1, 1, 2, 1, 1, 1, 1, 1, 1,// (7, 5) => 1 + w*5 : end
	};
	int path[100];// (1,1) => (2,1) => ... => (7,1) => (7,2) => ... => (7,5)
	int pathNum;// 11
	run(value, w, h, (1+w*1), (7+w*5), path, pathNum);
#endif
