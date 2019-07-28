#pragma once

#include <GL/glew.h>
//#include <GL/gl.h>
//#include <GL/glu.h>

#include "iType.h"

void initOpenGL();
void reshapeOpenGL();

typedef enum _iBlend {
	iBlendAlpha = 0,
	iBlendGrey,
	iBlendSolid,
} iBlend;

iBlend getGlBlend();
void setGlBlend(iBlend blend);

void applyTexParameters();
void setAntiAliasTexParameter(bool anti);
uint32 nextPOT(uint32 x);

#if SUPPORT_OPENGL_3

typedef struct _Vertex {
	float position[4];
	float color[4];
	float texcoord[2];
} Vertex;

extern Vertex _vertex_[4];

#include "iMatrix.h"
//struct iMatrix;
extern iMatrix* projMatrix;
extern iMatrix* viewMatrix;

extern GLuint projUniform;
extern GLuint viewUniform;
extern GLuint texUniform;

#endif




