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
#define BRICK_STATE_HITTING 101
#define BRICK_STATE_NORMAL 102
#define GOLD_BRICK_BOUNCING_SPEED 0.05f
#define GOLD_BRICK_GRAVITY	0.05f

class CCoinBrick : public CGameObject
{
private:
	float isBreakable = false;
	int breakCount = 1;
	int initialY = y;
	float old_pos, min_pos;
	ULONGLONG jump_start;
public:
	CCoinBrick(float x, float y, float isBreakable = false, int breakCount = 1) : CGameObject(x, y), isBreakable(isBreakable), breakCount(breakCount) {
		this->old_pos = y;
		this->min_pos = y - 4;
		this->jump_start = GetTickCount64();
		this->state = BRICK_STATE_NORMAL;
	}
	virtual void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void SpawnCoin();
	void Bounce();
	void OnNoCollision(DWORD dt);
	int IsCollidable() { return 1; }
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	int GetBreakCount() { return breakCount; }
	void SetBreakCOunt(int count) { breakCount = count; }
};

