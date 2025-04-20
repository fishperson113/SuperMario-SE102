#include "Pipe.h"
#include "Sprite.h"
#include "Sprites.h"

#include "Textures.h"
#include "Game.h"

void Pipe::Render()
{
	if (this->length <= 0) return;
	float yy = x;
	CSprites* s = CSprites::GetInstance();

	s->Get(this->spriteIdBot)->Draw(x, yy);
	yy += this->cellWidth;
	for (int i = 1; i < this->length - 1; i++)
	{
		s->Get(this->spriteIdBody)->Draw(x, yy);
		yy += this->cellWidth;
	}
	if (length > 1)
		s->Get(this->spriteIdTop)->Draw(x, yy);

	RenderBoundingBox();
}

void Pipe::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

void Pipe::RenderBoundingBox()
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

int Pipe::IsDirectionColliable(float nx, float ny)
{
	return 1;
}
