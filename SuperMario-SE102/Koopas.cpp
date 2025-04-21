#include "Koopas.h"

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_SHELL)
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else 
	{
		left = x - KOOPAS_BBOX_WIDTH / 2;
		top = y - KOOPAS_BBOX_HEIGHT / 2;
		right = left + KOOPAS_BBOX_WIDTH;
		bottom = top + KOOPAS_BBOX_HEIGHT;
	}
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (state == KOOPAS_STATE_SHELL && GetTickCount64() - shell_start > KOOPAS_SHELL_TIMEOUT)
	{
		SetState(KOOPAS_STATE_WALKING);
	}
}

void Koopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
	if (state == KOOPAS_STATE_WALKING)
	{
		if (vx > 0)
		{
			aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
		}
		else if (vx < 0)
		{
			aniId = ID_ANI_KOOPAS_WALKING_LEFT;
		}
	}
	else if (state == KOOPAS_STATE_SHELL)
	{
		aniId = ID_ANI_KOOPAS_SHELL;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	//RenderBoundingBox();
}

void Koopas::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void Koopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<Koopas*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
}

Koopas::Koopas(float x, float y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	shell_start = 0;

	SetState(KOOPAS_STATE_WALKING);
}

void Koopas::SetState(int state)
{
	int previousState = this->state;
	this->state = state;
	switch (state)
	{
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		if (previousState == KOOPAS_STATE_SHELL)
		{
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}
		break;

	case KOOPAS_STATE_SHELL:
		vx = 0;
		ay = KOOPAS_GRAVITY;

		if (previousState == KOOPAS_STATE_WALKING)
		{
			y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}

		shell_start = GetTickCount64();
		break;
	}
}
