#pragma once
#include "GameObject.h"
#include "Pipe.h"
#include "Game.h"
#include "PlayScene.h"
#include "Bullet.h"
#include "PiranhaPlant.h"

#define CHANGE_DIRECTION_TIME 8000
#define HIDING_TIME 10000
#define OFFSET -23
#define FIRETRAP_STATE_APPEARING 0
#define FIRETRAP_STATE_SHOOTING 1
#define FIRETRAP_STATE_HIDING 2
#define FIRETRAP_STATE_HIDDEN 3
#define TYPE_RED_FIRE_TRAP 0
#define TYPE_GREEN_FIRE_TRAP 1
#define TYPE_GREEN_PIRANHA_PLANT 2

class CFireTrap : public CGameObject
{
public:
	CFireTrap(float x, float y, int type);
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
	int state;
	int type = 0; //0: Red Fire Trap, 1: Green Fire Trap, 2: Green Piranha Plant
};

