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
	CBullet* bullet = new CBullet(x, y - 10);
	currentScene->GetObjectManager()->Add(bullet);

	LPGAMEOBJECT player = currentScene->GetPlayer();

	bool isLeft = false, isRight = false, isTop = false, isBottom = false, isHiding = false;

	float playerX = 0.0f;
	float playerY = 0.0f;

	if (player != nullptr)
	{
		player->GetPosition(playerX, playerY);
	}

	if (playerX < 376)
		isLeft = true;
	else
		isRight = true;

	if (playerY < 118)
		isTop = true;
	else
		isBottom = true;

	if (isLeft)
	{
		if (isTop)
			bullet->SetDirection(1);
		if (isBottom)
			bullet->SetDirection(3);
	}
	else if (isRight)
	{
		if (isTop)
			bullet->SetDirection(2);
		if (isBottom)
			bullet->SetDirection(4);
	}

}

void CPiranhaPlant::Update(DWORD dt)
{
	
}