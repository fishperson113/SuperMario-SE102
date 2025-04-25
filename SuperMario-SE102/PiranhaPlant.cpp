#include "PiranhaPlant.h"

CPiranhaPlant::CPiranhaPlant(float x, float y)
{
	CPipe* pipe = new CPipe(x, y, 31, 15, 3, 52001, 51001, 51001);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	currentScene->GetObjectManager()->Add(pipe);
	this->x = x;
	this->y = y;
}

void CPiranhaPlant::Render()
{
	int aniId = ID_ANI_PIRANHA_PLANT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y - 20);
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
	if (nx == 0 && ny == -1) return 1;
	return 0;
}

void CPiranhaPlant::ShootBullet()
{
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	CBullet* bullet = new CBullet(x, y - PIRANHA_PLANT_BBOX_HEIGHT / 2);
	currentScene->GetObjectManager()->Add(bullet);
}

void CPiranhaPlant::Update(DWORD dt)
{
	ShootBullet();
}