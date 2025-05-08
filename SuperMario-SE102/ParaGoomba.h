#pragma once
#include "GameObject.h"
#include "FallPitch.h"

#define PARAGOOMBA_GRAVITY 0.002f
#define PARAGOOMBA_WALKING_SPEED 0.05f
#define PARAGOOMBA_JUMP_SPEED -0.2f

#define PARAGOOMBAWINGS_BBOX_WIDTH 22
#define PARAGOOMBAWINGS_BBOX_HEIGHT 21
#define PARAGOOMBA_BBOX_HEIGHT 12
#define PARAGOOMBA_BBOX_WIDTH 18
#define PARAGOOMBA_BBOX_HEIGHT_DIE 17

#define PARAGOOMBA_STATE_WALKING 100
#define PARAGOOMBA_STATE_JUMPING 150
#define PARAGOOMBA_STATE_DIE 200
#define PARAGOOMBA_STATE_WALKING2 250

#define PARAGOOMBA_DIE_TIMEOUT 500

#define ID_ANI_PARAGOOMBA_WALKING 4999
#define ID_ANI_PARAGOOMBA_DIE 4998
#define ID_ANI_PARAGOOMBA_JUMPING 4997
#define ID_ANI_PARAGOOMBA_WALKING2 4996

class CParaGoomba : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG jump_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithFallPitch(LPCOLLISIONEVENT e);
public:
	CParaGoomba(float x, float y);
	virtual void SetState(int state);
};

