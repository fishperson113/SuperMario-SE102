#pragma once

#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "debug.h"

#define ID_ANI_COIN 11000

#define	COIN_WIDTH 10
#define COIN_BBOX_WIDTH 10
#define COIN_BBOX_HEIGHT 16

#define COIN_STATE_JUMP 1
#define COIN_JUMP_SPEED 0.1f
#define COIN_GRAVITY 0.001f
#define COIN_LIFETIME 500
#define COIN_JUMPTIME 200

class CCoin : public CGameObject {
protected:
	ULONGLONG spawnTime;
public:
	CCoin(float x, float y) : CGameObject(x, y) { spawnTime = GetTickCount64(); }
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int IsBlocking() { return 0; }
};