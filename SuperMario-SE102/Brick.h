#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"
#include "Switch.h"

#define ID_ANI_BRICK 10000
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16
#define BRICK_STATE_NORMAL 100
#define BRICK_STATE_HIT 200

class CBrick : public CGameObject {
	int brickNumber; // Number of the brick
	float offsetX;   // X offset
	float offsetY;   // Y offset
	int aniId; // Animation ID
	int bboxWidth; // Bounding box width
	int bboxHeight; // Bounding box height
	bool isBreakable = false;
	int coinCount = 0;
	bool ableToChangeToCoin = false;
	bool isCoin = false;
	bool justTurnedToCoin = false;
public:
	CBrick(float x, float y, int brickNumber = 1, float offsetX = 0.0f, float offsetY = 0.0f, int aniId = ID_ANI_BRICK, int bboxWidth = BRICK_BBOX_WIDTH, int bboxHeight = BRICK_BBOX_HEIGHT, bool isBreakable = false, int coinCount = 0)
		: CGameObject(x, y), brickNumber(brickNumber), offsetX(offsetX), offsetY(offsetY), aniId(aniId), bboxWidth(bboxWidth), bboxHeight(bboxHeight), isBreakable(isBreakable), coinCount(coinCount)
	{
		this->aniId = aniId;
		this->isBreakable = isBreakable;
		this->ableToChangeToCoin = ableToChangeToCoin;
		this->isCoin = false;
		this->justTurnedToCoin = false;
		this->bboxWidth = bboxWidth;
		this->bboxHeight = bboxHeight;
		this->offsetX = offsetX;
		this->offsetY = offsetY;
		this->state = BRICK_STATE_NORMAL;
	}

	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsBlocking() { return !isCoin; };
	int IsDirectionColliable(float nx, float ny);
	int IsBreakable() { return isBreakable; }
	int IsCollidable() { return isBreakable; }
	void SpawnBreakPiece();
	void Break();
	void SetAbleToChangeToCoin(bool able) { ableToChangeToCoin = able; }
	bool IsAbleToChangeToCoin() { return ableToChangeToCoin; }
	void TurnToCoin();
	bool IsCoin() { return isCoin; }
	bool JustTurnedToCoin() { return justTurnedToCoin; } 
	void ResetJustTurnedToCoin() { justTurnedToCoin = false; } 
	void SetState(int state);

};