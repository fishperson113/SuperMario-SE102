#pragma once
#include "GameObject.h"
#include "FallPitch.h"

#define KOOPA_PARATROOPA_GRAVITY 0.001f
#define KOOPA_PARATROOPA_WALKING_SPEED 0.05f
#define KOOPA_PARATROOPA_JUMP_SPEED -0.5f

#define KOOPA_PARATROOPA_WINGS_BBOX_WIDTH 18
#define KOOPA_PARATROOPA_WINGS_BBOX_HEIGHT 26
#define KOOPA_PARATROOPA_BBOX_HEIGHT 24
#define KOOPA_PARATROOPA_BBOX_WIDTH 18
#define KOOPA_PARATROOPA_SHELL_BBOX_HEIGHT 16
#define KOOPA_PARATROOPA_SHELL_BBOX_WIDTH 16
#define KOOPA_PARATROOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_PARATROOPA_STATE_WALKING_WINGS1 100
#define KOOPA_PARATROOPA_STATE_JUMPING_WINGS1 150
#define KOOPA_PARATROOPA_STATE_DIE1 200
#define KOOPA_PARATROOPA_STATE_WALKING1 250
#define KOOPA_PARATROOPA_STATE_SHELL1 300
#define KOOPA_PARATROOPA_STATE_SHELL_FEET1 350

#define KOOPA_PARATROOPA_DIE_TIMEOUT 500
#define KOOPA_PARATROOPA_SHELL_FEET_TIMEOUT 4000
#define KOOPA_PARATROOPA_SHELL_TIMEOUT 2000

#define ID_ANI_KOOPA_PARATROOPA_WALKING_WINGS_LEFT1 4993
#define ID_ANI_KOOPA_PARATROOPA_WALKING_WINGS_RIGHT1 4992
#define ID_ANI_KOOPA_PARATROOPA_SHELL1 4991
#define ID_ANI_KOOPA_PARATROOPA_SHELL_FEET1 4990
#define ID_ANI_KOOPA_PARATROOPA_WALKING_LEFT1 4989
#define ID_ANI_KOOPA_PARATROOPA_WALKING_RIGHT1 4988
#define ID_ANI_KOOPA_PARATROOPA_DIE1 4987


class CKoopaParatroopa : public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG jump_start;
	ULONGLONG walk_start;
	ULONGLONG shell_start;
	ULONGLONG shell_feet_start;

	int lastDirection = 0; // 0: left, 1: right

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithFallPitch(LPCOLLISIONEVENT e);
	void SetDirection(int direction) { this->lastDirection = direction; }
	int GetDirection() { return this->lastDirection; }
public:
	CKoopaParatroopa(float x, float y);
	virtual void SetState(int state);
};

