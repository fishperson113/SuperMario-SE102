#include "KoopaParatroopa.h"
#include "debug.h"

void CKoopaParatroopa::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPA_PARATROOPA_STATE_DIE)
	{
		left = x - KOOPA_PARATROOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_PARATROOPA_BBOX_HEIGHT_DIE / 2;
		right = left + KOOPA_PARATROOPA_BBOX_WIDTH;
		bottom = top + KOOPA_PARATROOPA_BBOX_HEIGHT_DIE;
	}
	else if (state == KOOPA_PARATROOPA_STATE_JUMPING_WINGS || state == KOOPA_PARATROOPA_STATE_WALKING_WINGS)
	{
		left = x - KOOPA_PARATROOPA_WINGS_BBOX_WIDTH / 2;
		top = y - KOOPA_PARATROOPA_WINGS_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARATROOPA_WINGS_BBOX_WIDTH;
		bottom = top + KOOPA_PARATROOPA_WINGS_BBOX_HEIGHT;
	}
	else if (state == KOOPA_PARATROOPA_STATE_WALKING)
	{
		left = x - KOOPA_PARATROOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_PARATROOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARATROOPA_BBOX_WIDTH;
		bottom = top + KOOPA_PARATROOPA_BBOX_HEIGHT;
	}
	else if (state = KOOPA_PARATROOPA_STATE_SHELL)
	{
		left = x - KOOPA_PARATROOPA_SHELL_BBOX_WIDTH / 2;
		top = y - KOOPA_PARATROOPA_SHELL_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARATROOPA_SHELL_BBOX_WIDTH;
		bottom = top + KOOPA_PARATROOPA_SHELL_BBOX_HEIGHT;
	}
	else
	{
		left = x - KOOPA_PARATROOPA_BBOX_WIDTH / 2;
		top = y - KOOPA_PARATROOPA_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARATROOPA_BBOX_WIDTH;
		bottom = top + KOOPA_PARATROOPA_BBOX_HEIGHT;
	}
}

void CKoopaParatroopa::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if (!isActive) return;
	vy += ay * dt;
	vx += ax * dt;

	if ((state == KOOPA_PARATROOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_PARATROOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}
	/*else if ((state == KOOPA_PARATROOPA_STATE_SHELL_FEET) && (GetTickCount64() - shell_feet_start > KOOPA_PARATROOPA_SHELL_FEET_TIMEOUT))
	{
		this->SetPosition(this->x, this->y - 10);
		this->SetState(KOOPA_PARATROOPA_STATE_WALKING);
		shell_feet_start = 0;
	}*/
	else if ((state == KOOPA_PARATROOPA_STATE_SHELL) && (GetTickCount64() - shell_start > KOOPA_PARATROOPA_SHELL_TIMEOUT))
	{
		//this->SetState(KOOPA_PARATROOPA_STATE_SHELL_FEET);
		this->SetPosition(this->x, this->y - 10);
		this->SetState(KOOPA_PARATROOPA_STATE_WALKING);
		shell_start = 0;
	}
	else if (state == KOOPA_PARATROOPA_STATE_WALKING_WINGS && GetTickCount64() - walk_start > 2000) // Walk duration
	{
		SetState(KOOPA_PARATROOPA_STATE_JUMPING_WINGS);
	}
	else if (state == KOOPA_PARATROOPA_STATE_JUMPING_WINGS && GetTickCount64() - jump_start > 1000) // Jump duration
	{
		SetState(KOOPA_PARATROOPA_STATE_WALKING_WINGS);
	}
	if (this->vx > 0)
		this->SetDirection(1);
	else
		this->SetDirection(0);
}

void CKoopaParatroopa::Render()
{
	int aniId = ID_ANI_KOOPA_PARATROOPA_WALKING_WINGS_LEFT;

	if (state == KOOPA_PARATROOPA_STATE_SHELL)
	{
		aniId = ID_ANI_KOOPA_PARATROOPA_SHELL;
	}
	else if (state == KOOPA_PARATROOPA_STATE_SHELL_FEET)
	{
		aniId = ID_ANI_KOOPA_PARATROOPA_SHELL_FEET;
	}
	else if (state == KOOPA_PARATROOPA_STATE_WALKING)
	{
		if (vx > 0)
		{
			aniId = ID_ANI_KOOPA_PARATROOPA_WALKING_RIGHT;
		}
		else if (vx < 0)
		{
			aniId = ID_ANI_KOOPA_PARATROOPA_WALKING_LEFT;
		}
	}
	else if (state == KOOPA_PARATROOPA_STATE_WALKING_WINGS || state == KOOPA_PARATROOPA_STATE_JUMPING_WINGS)
	{
		if (vx > 0)
		{
			aniId = ID_ANI_KOOPA_PARATROOPA_WALKING_WINGS_RIGHT;
		}
		else if (vx < 0)
		{
			aniId = ID_ANI_KOOPA_PARATROOPA_WALKING_WINGS_LEFT;
		}
	}
	else if (state == KOOPA_PARATROOPA_STATE_DIE)
	{
		aniId = ID_ANI_KOOPA_PARATROOPA_DIE;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void CKoopaParatroopa::OnNoCollision(DWORD dt)
{
	if (!isActive) return;
	x += vx * dt;
	y += vy * dt;
}

void CKoopaParatroopa::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;
	if (dynamic_cast<CKoopaParatroopa*>(e->obj)) return;

	if (e->ny != 0)
	{
		vy = 0;
	}
	else if (e->nx != 0)
	{
		vx = -vx;
	}
	if (dynamic_cast<CFallPitch*>(e->obj))
		OnCollisionWithFallPitch(e);
}

void CKoopaParatroopa::OnCollisionWithFallPitch(LPCOLLISIONEVENT e)
{
	this->SetState(KOOPA_PARATROOPA_STATE_DIE);
}

CKoopaParatroopa::CKoopaParatroopa(float x, float y) : CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPA_PARATROOPA_GRAVITY;
	this->lastDirection = 0;
	die_start = -1;
	jump_start = -1;
	walk_start = -1;
	shell_start = -1;
	shell_feet_start = -1;
	SetState(KOOPA_PARATROOPA_STATE_WALKING_WINGS);
}

void CKoopaParatroopa::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPA_PARATROOPA_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
		break;
	case KOOPA_PARATROOPA_STATE_WALKING_WINGS:
		walk_start = GetTickCount64();
		if (this->GetDirection() == 0)
		{
			vx = -KOOPA_PARATROOPA_WALKING_SPEED;
		}
		else
		{
			vx = KOOPA_PARATROOPA_WALKING_SPEED;
		}
		break;
	case KOOPA_PARATROOPA_STATE_JUMPING_WINGS:
		vy = KOOPA_PARATROOPA_JUMP_SPEED;
		jump_start = GetTickCount64();
		break;
	case KOOPA_PARATROOPA_STATE_WALKING:
		if (this->GetDirection() == 0)
		{
			vx = -KOOPA_PARATROOPA_WALKING_SPEED;
		}
		else
		{
			vx = KOOPA_PARATROOPA_WALKING_SPEED;
		}
		break;
	case KOOPA_PARATROOPA_STATE_SHELL:
		shell_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	case KOOPA_PARATROOPA_STATE_SHELL_FEET:
		shell_feet_start = GetTickCount64();
		vx = 0;
		vy = 0;
		break;
	}
}
