#pragma once
#include "GameObject.h"

#define MUSHROOM_WALKING_SPEED 0.05f
#define MUSHROOM_GRAVITY 0.002f

#define MUSHROOM_BBOX_WIDTH 17
#define MUSHROOM_BBOX_HEIGHT 17

#define MUSHROOM_STATE_WALKING 101

#define ID_ANI_MUSHROOM_WALKING 90002

class CMushroom : public CGameObject
{
	bool isCollected;
protected:
	float ax;
	float ay;

	ULONGLONG die_start;

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);

	virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
	CMushroom(float x, float y);
	virtual void SetState(int state);
	bool IsCollected() const { return isCollected; }
	void SetCollected(bool collected) { isCollected = collected; }
	void BeCollected();
};


