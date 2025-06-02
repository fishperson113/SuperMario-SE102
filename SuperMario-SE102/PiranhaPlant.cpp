#include "PiranhaPlant.h"

void CPiranhaPlant::HandlePlantDeadEffect()
{
	die_start = GetTickCount64();

	float effectY = this->y;
	if (this->type == 0)
		effectY = y - PIRANHA_PLANT_BBOX_HEIGHT / 2 + 3.0f;
	else
		effectY = y - GREEN_FIRE_TRAP_BBOX_HEIGHT / 2 + 3.0f;

	eff_die = new CEffectHit(x, effectY, die_start, EFF_COL_TYPE_SMOKE_EVOLVE);

	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	current_scene->GetObjectManager()->Add(eff_die);
}

CPiranhaPlant::CPiranhaPlant(float x, float y)
{
	this->spawnTime = GetTickCount64();
	this->die_start = 0;
	this->eff_die = nullptr;
	this->x = x;
	this->y = y;
}
void CPiranhaPlant::SetState(int state)
{
	switch (state)
	{
	case PIRANHA_PLANT_STATE_DIE:
		this->Delete();
		HandlePlantDeadEffect();
		break;
	}

	CGameObject::SetState(state);
}
void CPiranhaPlant::Render()
{
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (this->type == 0)
	{
		l = x - PIRANHA_PLANT_BBOX_WIDTH / 2;
		t = y - PIRANHA_PLANT_BBOX_HEIGHT / 2;
		r = l + PIRANHA_PLANT_BBOX_WIDTH;
		b = t + PIRANHA_PLANT_BBOX_HEIGHT;
	}
	else
	{
		l = x - GREEN_FIRE_TRAP_BBOX_WIDTH / 2;
		t = y - GREEN_FIRE_TRAP_BBOX_HEIGHT / 2;
		r = l + GREEN_FIRE_TRAP_BBOX_WIDTH;
		b = t + GREEN_FIRE_TRAP_BBOX_HEIGHT;

	}
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