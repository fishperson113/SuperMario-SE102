#pragma once
#include "GameObject.h"
#include "Pipe.h"
#include "Game.h"
#include "PlayScene.h"
#include "Bullet.h"

#define PIRANHA_PLANT_BBOX_WIDTH 17
#define PIRANHA_PLANT_BBOX_HEIGHT 60
#define ID_ANI_PIRANHA_PLANT 200006
class CPiranhaPlant : public CGameObject
{
public:
	CPiranhaPlant(float x, float y);
	void Render();
	void Update(DWORD dt);
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	int IsDirectionColliable(float nx, float ny);
	void ShootBullet();
protected:
	ULONGLONG spawnTime;
};

