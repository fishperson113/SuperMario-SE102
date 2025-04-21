#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Coin.h"
#include "PlayScene.h"

#define ID_ANI_COINBRICK 10001
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16
#define BRICK_STATE_HIT 100

class CCoinBrick : public CGameObject
{
public:
	CCoinBrick(float x, float y) : CGameObject(x, y) {}
	virtual void Render();
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void SpawnCoin();
};

