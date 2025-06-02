#pragma once

#include "Animation.h"
#include "Animations.h"
#include "GameObject.h"
#include "FallSensor.h"
#define KOOPAS_GRAVITY 0.0015f
#define KOOPAS_WALKING_SPEED 0.05f
#define KOOPAS_SHELL_SPEED 0.2f
// States
#define KOOPAS_STATE_WALKING 100
#define KOOPAS_STATE_SHELL 200
#define KOOPAS_STATE_SHELL_MOVING 300
#define KOOPAS_STATE_DIE 400
#define KOOPAS_STATE_UPSIDE_DOWN 500

#define KOOPAS_BBOX_WIDTH 18
#define KOOPAS_BBOX_HEIGHT 25
#define KOOPAS_BBOX_HEIGHT_SHELL 25

#define ID_ANI_KOOPAS_WALKING_RIGHT 6003
#define ID_ANI_KOOPAS_WALKING_LEFT 6002
#define ID_ANI_KOOPAS_SHELL 6004
#define ID_ANI_KOOPAS_UPSIDE_DOWN 6005

#define KOOPAS_SHELL_DECELERATION 0.000f  
#define KOOPAS_MIN_SPEED 0.01f
#define KOOPAS_SHELL_TIMEOUT 5000

#define KOOPAS_RED 0
#define KOOPAS_GREEN_NO_WINGS 1
#define KOOPAS_GREEN 2
#define KOOPAS_RED_WINGS 3

#define KOOPA_PARATROOPA_GRAVITY 0.002f
#define KOOPA_PARATROOPA_WALKING_SPEED 0.03f
#define KOOPA_PARATROOPA_JUMP_SPEED -0.4f

#define KOOPA_PARATROOPA_WINGS_BBOX_WIDTH 18
#define KOOPA_PARATROOPA_WINGS_BBOX_HEIGHT 26
#define KOOPA_PARATROOPA_BBOX_HEIGHT 25
#define KOOPA_PARATROOPA_BBOX_WIDTH 18
#define KOOPA_PARATROOPA_SHELL_BBOX_HEIGHT 25
#define KOOPA_PARATROOPA_SHELL_BBOX_WIDTH 18
#define KOOPA_PARATROOPA_BBOX_HEIGHT_DIE 16

#define KOOPA_PARATROOPA_STATE_WALKING_WINGS 101
#define KOOPA_PARATROOPA_STATE_JUMPING_WINGS 151
#define KOOPA_PARATROOPA_STATE_DIE 201
#define KOOPA_PARATROOPA_STATE_WALKING 251
#define KOOPA_PARATROOPA_STATE_SHELL 301
#define KOOPA_PARATROOPA_STATE_SHELL_FEET 351
#define KOOPA_PARATROOPA_STATE_MOVING_SHELL 401

#define KOOPAS_STATE_FLOATING 402

#define KOOPA_PARATROOPA_DIE_TIMEOUT 500
#define KOOPA_PARATROOPA_SHELL_FEET_TIMEOUT 4000
#define KOOPA_PARATROOPA_SHELL_TIMEOUT 5000

#define ID_ANI_KOOPA_PARATROOPA_WALKING_WINGS_LEFT 4993
#define ID_ANI_KOOPA_PARATROOPA_WALKING_WINGS_RIGHT 4992
#define ID_ANI_KOOPA_PARATROOPA_SHELL 4991
#define ID_ANI_KOOPA_PARATROOPA_SHELL_FEET 4990
#define ID_ANI_KOOPA_PARATROOPA_WALKING_LEFT 4989
#define ID_ANI_KOOPA_PARATROOPA_WALKING_RIGHT 4988
#define ID_ANI_KOOPA_PARATROOPA_DIE 4987

#define ID_ANI_RED_WALKING_WINGS_LEFT 4986


class Koopas :public CGameObject
{
private:
	FallSensor* fallSensor;
	bool isBeingHeld;
	int type = 0;
	int lastDirection = 0; // 0: left, 1: right
	float initialY;
	float floatTime;
	int preState;
protected:
	float ax;
	float ay;

	ULONGLONG die_start;
	ULONGLONG shell_start;
	ULONGLONG jump_start;
	ULONGLONG walk_start;

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
	void OnCollisionWithLeafBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithBrick(LPCOLLISIONEVENT e);

public:
	Koopas(float x, float y, int type);
	virtual void SetState(int state);
	void ChangeDirection();
	void ResetSensors();

	void SetBeingHeld(bool held) { isBeingHeld = held; }
	bool IsBeingHeld() { return isBeingHeld; }
	bool IsAboutToWakeUp() 
	{
		if (this->type == KOOPAS_RED)
			return state == KOOPAS_STATE_SHELL && GetTickCount64() - shell_start > KOOPAS_SHELL_TIMEOUT; 
		else
			return state == KOOPA_PARATROOPA_STATE_SHELL && GetTickCount64() - shell_start > KOOPA_PARATROOPA_SHELL_TIMEOUT;
	}
	int GetType() { return this->type; }
	void SetDirection(int direction) { this->lastDirection = direction; }
	int GetDirection() { return this->lastDirection; }

	FallSensor* GetFallSensor() { return fallSensor; }
	void SetType(int type) { this->type = type; }
};

