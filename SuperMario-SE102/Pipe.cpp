#include "Pipe.h"

#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void CPipe::Render()
{
	if (this->length <= 0) return;
	float yy = y;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBegin)->Draw(x, yy);
	yy += this->cellHeight;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdMiddle)->Draw(x, yy);
		yy += this->cellHeight;
	}
	if (length > 1)
		s->Get(this->spriteIdEnd)->Draw(x, yy);

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	float cellWidth_div_2 = this->cellWidth / 2;
	l = x - cellWidth_div_2;
	t = y - this->cellHeight / 2;
	r = l + this->cellWidth;
	b = t + this->cellHeight * this->length;
}

void CPipe::RenderBoundingBox()
{
	D3DXVECTOR3 p(x, y, 0);
	RECT rect;

	LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

	float l, t, r, b;

	GetBoundingBox(l, t, r, b);
	rect.left = 0;
	rect.top = 0;
	rect.right = (int)r - (int)l;
	rect.bottom = (int)b - (int)t;

	float cx, cy;
	CGame::GetInstance()->GetCamPos(cx, cy);

	float xx = x - this->cellWidth / 2 + rect.right / 2;

	CGame::GetInstance()->Draw(xx - cx, y - cy, bbox, nullptr, BBOX_ALPHA, rect.right - 1, rect.bottom - 1);
}

int CPipe::IsDirectionColliable(float nx, float ny)
{
	if (nx == 0 && ny == -1) return 1;
	if (nx != 0) return 1;
	return 0;
}

void CPipe::SetAsEntrance(float targetX, float targetY, PipeDirection entryDir, PipeDirection exitDir)
{
	this->isEntrance = true;
	this->isExit = false;
	this->targetX = targetX;
	this->targetY = targetY;
	this->entryDirection = entryDir;
	this->exitDirection = exitDir;
}

void CPipe::SetAsExit()
{
	this->isExit = true;
	this->isEntrance = false;
}

void CPipe::GetTargetPosition(float& x, float& y) const
{
	x = this->targetX;
	y = this->targetY;
}

void CPipe::GetEntryPosition(float& x, float& y) const
{
	// Return the position based on entry direction
	x = this->x; // Default to pipe center

	switch (entryDirection)
	{
	case PipeDirection::UP:
		// If entering from below, position at the bottom of the pipe
		y = this->y + (this->cellHeight * this->length) / 2;
		break;

	case PipeDirection::DOWN:
		// If entering from above, position at the top of the pipe
		y = this->y - this->cellHeight / 2;
		break;

	case PipeDirection::LEFT:
		// For horizontal entry from right side
		x = this->x + this->cellWidth / 2;
		y = this->y;
		break;

	case PipeDirection::RIGHT:
		// For horizontal entry from left side
		x = this->x - this->cellWidth / 2;
		y = this->y;
		break;

	default:
		y = this->y;
		break;
	}
}

void CPipe::GetExitPosition(float& x, float& y) const
{
	x = this->x; // Default to pipe center

	if (isExit)
	{
		// For exit pipes, calculate based on the pipe's own direction
		switch (entryDirection)
		{
		case PipeDirection::UP:
			// When exiting upward, position above the pipe
			y = this->y - this->cellHeight / 2;
			break;

		case PipeDirection::DOWN:
			// When exiting downward, position below the pipe
			y = this->y + (this->cellHeight * this->length) / 2;
			break;

		case PipeDirection::LEFT:
			// When exiting leftward
			x = this->x - this->cellWidth / 2;
			y = this->y;
			break;

		case PipeDirection::RIGHT:
			// When exiting rightward
			x = this->x + this->cellWidth / 2;
			y = this->y;
			break;

		default:
			y = this->y;
			break;
		}
	}
	else
	{
		// Default fallback
		y = this->y;
	}
}
