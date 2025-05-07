#include "ParaGoomba.h"
#include "debug.h"
#define PARAGOOMBA_STATE_DIE 200

void CParaGoomba::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == PARAGOOMBA_STATE_DIE)
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - PARAGOOMBA_BBOX_HEIGHT_DIE / 2;
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT_DIE;
	}
	else if (state == PARAGOOMBA_STATE_JUMPING || state == PARAGOOMBA_STATE_WALKING)
	{
		left = x - PARAGOOMBAWINGS_BBOX_WIDTH / 2;
		top = y - PARAGOOMBAWINGS_BBOX_HEIGHT / 2;
		right = left + PARAGOOMBAWINGS_BBOX_WIDTH;
		bottom = top + PARAGOOMBAWINGS_BBOX_HEIGHT;
	}
	else
	{
		left = x - PARAGOOMBA_BBOX_WIDTH / 2;
		top = y - PARAGOOMBA_BBOX_HEIGHT / 2;
		right = left + PARAGOOMBA_BBOX_WIDTH;
		bottom = top + PARAGOOMBA_BBOX_HEIGHT;
	}
}

void CParaGoomba::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (!isActive) return;
	vy += ay * dt;
	vx += ax * dt;

	if ((state == PARAGOOMBA_STATE_DIE) && (GetTickCount64() - die_start > PARAGOOMBA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	DebugOut(L"ParaGoomba: %f, %f\n", x, y);
	if (state == PARAGOOMBA_STATE_JUMPING && GetTickCount64() - jump_start > 500) // Jump duration
	{
		SetState(PARAGOOMBA_STATE_WALKING);
	}
	if (state == PARAGOOMBA_STATE_WALKING && GetTickCount64() - jump_start > 500) // Walk duration
	{
		SetState(PARAGOOMBA_STATE_JUMPING);
	}
}

void CParaGoomba::Render()
{
	int aniId = ID_ANI_PARAGOOMBA_WALKING;
	if (state == PARAGOOMBA_STATE_DIE)
	{
		aniId = ID_ANI_PARAGOOMBA_DIE;
	}
	else if (state == PARAGOOMBA_STATE_JUMPING)
	{
		aniId = ID_ANI_PARAGOOMBA_JUMPING;
	}
	else if (state == PARAGOOMBA_STATE_WALKING2)
	{
		aniId = ID_ANI_PARAGOOMBA_WALKING2;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CParaGoomba::OnNoCollision(DWORD dt)
{
	if (!isActive) return;
	x += vx * dt;
	y += vy * dt;
}

void CParaGoomba::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CParaGoomba*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

CParaGoomba::CParaGoomba(float x, float y) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = PARAGOOMBA_GRAVITY;
	die_start = -1;
	jump_start = -1;
	SetState(PARAGOOMBA_STATE_WALKING);
}

void CParaGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case PARAGOOMBA_STATE_DIE:
		die_start = GetTickCount64();
		y += (PARAGOOMBA_BBOX_HEIGHT - PARAGOOMBA_BBOX_HEIGHT_DIE) / 2;
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case PARAGOOMBA_STATE_WALKING:
		vx = -PARAGOOMBA_WALKING_SPEED;
		break;
	case PARAGOOMBA_STATE_JUMPING:
		vy = PARAGOOMBA_JUMP_SPEED;
		jump_start = GetTickCount64();
		break;
	}
}
