#include "Brick.h"

void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	for (int i = 0; i < brickNumber; i++)
	{
		float posX = x + i * offsetX;
		float posY = y + i * offsetY;
		animations->Get(this->aniId)->Render(posX,posY);
	}
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - this->bboxWidth / 2 + offsetX; 
	t = y - this->bboxHeight / 2 + offsetY;
	r = l + this->bboxWidth;
	b = t + this->bboxHeight;
}

int CBrick::IsDirectionColliable(float nx, float ny)
{
	if (this->aniId != 10000 && this->aniId != 10014)
	{
		if (nx == 0 && ny == -1) return 1;
		else return 0;
	}
	else
		return 1;
}
