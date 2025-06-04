#pragma once
#include "GameObject.h"
#define ID_ANI_BULLET_FLYING 4995
#define BULLET_BBOX_WIDTH 9
#define BULLET_BBOX_HEIGHT 9
#define BULLET_TIMEOUT 5000 // 3 seconds

class CBullet : public CGameObject
{
protected:
	float ax;
	float ay;
	int direction;
	ULONGLONG spawnTime;
public:
	CBullet(float x, float y);
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

	virtual int IsCollidable() { return 1; };
	virtual int IsBlocking() { return 0; }
	virtual void OnNoCollision(DWORD dt);
	virtual void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnCollisionWithMario(LPCOLLISIONEVENT e);
	void SetDirection(int direction) { this->direction = direction; }
	int GetDirection() { return direction; }
};

