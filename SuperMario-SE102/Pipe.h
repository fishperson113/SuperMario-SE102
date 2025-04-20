#pragma once
#include "GameObject.h"
class Pipe : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBot, spriteIdBody, spriteIdTop;

public:
	Pipe(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_bot, int sprite_id_body, int sprite_id_top) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBot = sprite_id_bot;
		this->spriteIdBody = sprite_id_body;
		this->spriteIdTop = sprite_id_top;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);
};

