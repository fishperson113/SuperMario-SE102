#pragma once

#include "Animation.h"
#include "Animations.h"
#include "GameObject.h"

#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f

// States
#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200

#define KOOPAS_BBOX_WIDTH 18
#define KOOPAS_BBOX_HEIGHT 25
#define KOOPAS_BBOX_HEIGHT_SHELL 23

#define ID_ANI_KOOPAS_WALKING_RIGHT 6003
#define ID_ANI_KOOPAS_WALKING_LEFT 6002
#define ID_ANI_KOOPAS_SHELL 6004

#define KOOPAS_SHELL_TIMEOUT 5000
class Koopas :public CGameObject
{
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG shell_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	Koopas(float x, float y);
	virtual void SetState(int state);
};

