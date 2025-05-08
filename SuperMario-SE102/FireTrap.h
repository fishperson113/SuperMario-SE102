#pragma once
#include "GameObject.h"
#include "Pipe.h"
#include "Game.h"
#include "PlayScene.h"
#include "Bullet.h"
#include "PiranhaPlant.h"

#define SHOOTING_TIME 4000
#define CHANGE_DIRECTION_TIME 8000
#define HIDING_TIME 10000
#define OFFSET -23

class CFireTrap : public CGameObject
{
public:
	CFireTrap(float x, float y);
	void Render();
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny);
	void ShootBullet();
protected:
	ULONGLONG spawnTime;
	CPiranhaPlant* piranhaPlant;
	CPipe* pipe;
	float offset = 0.0f;
	float offset2 = 0.0f;
	bool hasFired = false;
};

