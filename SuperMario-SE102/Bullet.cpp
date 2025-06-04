#include "Bullet.h"
#include "Mario.h"

CBullet::CBullet(float x, float y)
{
	this->x = x;
	this->y = y;
	spawnTime = GetTickCount64();
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_BBOX_WIDTH / 2;
	top = y - BULLET_BBOX_HEIGHT / 2;
	right = left + BULLET_BBOX_WIDTH;
	bottom = top + BULLET_BBOX_HEIGHT;

}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (GetTickCount64() - spawnTime > BULLET_TIMEOUT)
	{
		isDeleted = true;
		return;
	}
}

void CBullet::Render()
{
	int aniId = ID_ANI_BULLET_FLYING;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CBullet::OnNoCollision(DWORD dt)
{
	if (this->direction == 1)
	{
		x += -0.02f * dt;
		y += -0.02f * dt;
	}
	else if (this->direction == 2)
	{
		x += 0.02f * dt;
		y += -0.02f * dt;
	}
	else if (this->direction == 3)
	{
		x += -0.02f * dt;
		y += 0.02f * dt;
	}
	else if (this->direction == 4)
	{
		x += 0.02f * dt;
		y += 0.02f * dt;
	}
}

void CBullet::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (dynamic_cast<CMario*>(e->obj))
		OnCollisionWithMario(e);
}

void CBullet::OnCollisionWithMario(LPCOLLISIONEVENT e)
{
	DebugOut(L"OnCollisionWithBullet \n");

	CMario* mario = (CMario*)e->obj;

	if (mario->GetUntouchable() == 0)
	{
		if (mario->GetLevel() > MARIO_LEVEL_SMALL)
		{
			mario->SetLevel(MARIO_LEVEL_SMALL);
			mario->StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			mario->SetState(MARIO_STATE_DIE);
		}
	}
}