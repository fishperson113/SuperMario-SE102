#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CBrick : public CGameObject {
	int brickNumber; // Number of the brick
	float offsetX;   // X offset
	float offsetY;   // Y offset
	int aniId; // Animation ID
	int bboxWidth; // Bounding box width
	int bboxHeight; // Bounding box height
public:
	CBrick(float x, float y, int brickNumber = 1, float offsetX = 0.0f, float offsetY = 0.0f, float aniId = ID_ANI_BRICK, int bboxWidth = BRICK_BBOX_WIDTH, int bboxHeight = BRICK_BBOX_HEIGHT)
		: CGameObject(x, y), brickNumber(brickNumber), offsetX(offsetX), offsetY(offsetY), aniId(aniId), bboxWidth(bboxWidth), bboxHeight(bboxHeight){
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny);
};