#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	for (int i = 0; i < brickNumber; i++)
	{
		float posX = x + i * offsetX;
		float posY = y + i * offsetY;
		animations->Get(ID_ANI_BRICK)->Render(posX,posY);
	}
	//RenderBoundingBox();
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - BRICK_BBOX_WIDTH / 2 + offsetX; 
	t = y - BRICK_BBOX_HEIGHT / 2 + offsetY;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}