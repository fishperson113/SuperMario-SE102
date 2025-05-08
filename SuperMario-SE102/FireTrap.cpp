#include "FireTrap.h"

CFireTrap::CFireTrap(float x, float y) : CGameObject(x, y)
{
	piranhaPlant = new CPiranhaPlant(x, y + 10);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	currentScene->GetObjectManager()->Add(piranhaPlant);

	pipe = new CPipe(x, y, 31, 15, 3, 52001, 51001, 51001);

	currentScene->GetObjectManager()->Add(pipe);

	this->spawnTime = GetTickCount64();
}

void CFireTrap::Render()
{
}

void CFireTrap::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	ULONGLONG currentTime = GetTickCount64();
	if (currentTime - spawnTime > SHOOTING_TIME)
	{
		if (this->offset > OFFSET)
		{
			this->offset -= 0.5f;
			this->piranhaPlant->SetPosition(this->x, this->y + this->offset);
		}
		else
		{
			if (!this->hasFired)
			{
				this->piranhaPlant->ShootBullet();
				this->hasFired = true;
			}
		}
	}

	if (currentTime - spawnTime > CHANGE_DIRECTION_TIME)
	{
		this->piranhaPlant->SetAnimationId(ID_ANI_PIRANHA_CHANGE_DIRECTION);
	}

	if (currentTime - spawnTime > HIDING_TIME)
	{
		if (this->offset2 < -OFFSET - 1)
		{

			this->offset2 += 0.5f;
			this->piranhaPlant->SetPosition(this->x, this->y + this->offset2);
		}
		else
		{
			this->piranhaPlant->SetActive(false);
		}
	}
}

void CFireTrap::GetBoundingBox(float& l, float& t, float& r, float& b)
{
}

int CFireTrap::IsDirectionColliable(float nx, float ny)
{
	return 0;
}

