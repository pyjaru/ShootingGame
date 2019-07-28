#include "Map.h"

Texture* texBackground;
float mapPositions[3] = { 0, 0, 0, };

void loadMap()
{
	Texture* tex = createImage("assets/textures/background/background_1.png");
	texBackground = tex;

	for (int i = 0; i < 3; i++)
		mapPositions[i] = devSize.width*i;
}

void drawMap(float dt)
{
	float movement = dt * devSize.width / 5;
	for (int i = 0; i < 3; i++)
	{
		mapPositions[i] -= movement;
		if (mapPositions[i] < -devSize.width)
			mapPositions[i] += (devSize.width * 2);

		setRGBA(1, 1, 1, 1);
		drawImage(texBackground, mapPositions[i], 0, devSize.width, devSize.height, TOP | LEFT);
	}
	return;
}

void freeMap()
{
	free(texBackground);
}
