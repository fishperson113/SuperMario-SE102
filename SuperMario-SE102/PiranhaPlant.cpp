#include "PiranhaPlant.h"

CPiranhaPlant::CPiranhaPlant(float x, float y)
{
	this->spawnTime = GetTickCount64();

	this->x = x;
	this->y = y;
}

void CPiranhaPlant::Render()
{
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIRANHA_PLANT_BBOX_WIDTH / 2;
	t = y - PIRANHA_PLANT_BBOX_HEIGHT / 2;
	r = l + PIRANHA_PLANT_BBOX_WIDTH;
	b = t + PIRANHA_PLANT_BBOX_HEIGHT;
}

int CPiranhaPlant::IsDirectionColliable(float nx, float ny)
{
	return 1;
}

void CPiranhaPlant::ShootBullet()
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CBullet* bullet = new CBullet(x, y - PIRANHA_PLANT_BBOX_HEIGHT / 2);
	currentScene->GetObjectManager()->Add(bullet);
}

void CPiranhaPlant::Update(DWORD dt)
{
	
}