#pragma once
#include "GameObject.h"
#include "Switch.h"
#include "Animation.h"
#include "Animations.h"

#include "PlayScene.h"

#define SWITCH_BRICK_STATE_HIT 100
#define SWITCH_BRICK_BBOX_WIDTH 16
#define SWITCH_BRICK_BBOX_HEIGHT 16

class CSwitchBrick : public CGameObject
{
public:
	CSwitchBrick(float x, float y) : CGameObject(x, y) {}
	virtual void Render();
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int IsCollidable() { return 1; }
	void SpawnSwitch();
};

