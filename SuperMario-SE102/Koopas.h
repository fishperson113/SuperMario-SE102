#pragma once

#include "Animation.h"
#include "Animations.h"
#include "GameObject.h"
#include "FallSensor.h"
#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.2f
// States
#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200
#define KOOPAS_STATE_SHELL_MOVING 300

#define KOOPAS_BBOX_WIDTH 18
#define KOOPAS_BBOX_HEIGHT 25
#define KOOPAS_BBOX_HEIGHT_SHELL 25

#define ID_ANI_KOOPAS_WALKING_RIGHT 6003
#define ID_ANI_KOOPAS_WALKING_LEFT 6002
#define ID_ANI_KOOPAS_SHELL 6004

#define KOOPAS_SHELL_DECELERATION 0.0001f  
#define KOOPAS_MIN_SPEED 0.01f
#define KOOPAS_SHELL_TIMEOUT 5000

class Koopas :public CGameObject
{
private:
	FallSensor* fallSensor;
	bool isBeingHeld;
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
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithOtherKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithCoinBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroomBrick(LPCOLLISIONEVENT e);

public:
	Koopas(float x, float y);
	virtual void SetState(int state);
	void ChangeDirection();
	void ResetSensors();

	void SetBeingHeld(bool held) { isBeingHeld = held; }
	bool IsBeingHeld() { return isBeingHeld; }
	bool IsAboutToWakeUp() { return state == KOOPAS_STATE_SHELL && GetTickCount64() - shell_start > KOOPAS_SHELL_TIMEOUT; }

	FallSensor* GetFallSensor() { return fallSensor; }
};

