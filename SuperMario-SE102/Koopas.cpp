#include "Koopas.h"
#include "Goomba.h"
#include "CoinBrick.h"
#include "MushroomBrick.h"
void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELL_MOVING)
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
	if (!isBeingHeld)  
	{
		vy += ay * dt;
		vx += ax * dt;

		if (state == KOOPAS_STATE_SHELL_MOVING)
		{
			// Apply deceleration to the shell on left or right movement
			if (vx > 0)
			{
				vx -= KOOPAS_SHELL_DECELERATION * dt;
				if (vx < KOOPAS_MIN_SPEED)
				{
					vx = 0;
					SetState(KOOPAS_STATE_SHELL);
				}
			}
			else if (vx < 0)
			{
				vx += KOOPAS_SHELL_DECELERATION * dt;
				if (vx > -KOOPAS_MIN_SPEED)
				{
					vx = 0;
					SetState(KOOPAS_STATE_SHELL);
				}
			}
		}
	}
	if (IsAboutToWakeUp())
	{
		SetState(KOOPAS_STATE_WALKING);
	}
	
	if (!isBeingHeld && state == KOOPAS_STATE_WALKING)
	{
		if (fallSensor) fallSensor->Update(dt, coObjects);
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
	else if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELL_MOVING)
	{
		aniId = ID_ANI_KOOPAS_SHELL;
	}

	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void Koopas::OnNoCollision(DWORD dt)
{
	if (!isBeingHeld)  
	{
		x += vx * dt;
		y += vy * dt;
	}
}

void Koopas::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
			ResetSensors();
		}
	}

	if (state != KOOPAS_STATE_SHELL_MOVING)
		return;

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<Koopas*>(e->obj))
		OnCollisionWithOtherKoopas(e);
	else if (dynamic_cast<CCoinBrick*>(e->obj))
		OnCollisionWithCoinBrick(e);
	else if (dynamic_cast<CMushroomBrick*>(e->obj))
		OnCollisionWithMushroomBrick(e);
}

void Koopas::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
	if (goomba->GetState() != GOOMBA_STATE_DIE)
	{
		goomba->SetState(GOOMBA_STATE_DIE);
		DebugOut(L">>> Koopa shell killed Goomba! >>> \n");
	}
}

void Koopas::OnCollisionWithOtherKoopas(LPCOLLISIONEVENT e)
{
	Koopas* otherKoopas = dynamic_cast<Koopas*>(e->obj);
	if (otherKoopas->GetState() != KOOPAS_STATE_SHELL &&
		otherKoopas->GetState() != KOOPAS_STATE_SHELL_MOVING)
	{
		otherKoopas->SetState(KOOPAS_STATE_SHELL);
		DebugOut(L">>> Koopa shell hit another Koopa! >>> \n");
	}
}

void Koopas::OnCollisionWithCoinBrick(LPCOLLISIONEVENT e)
{
	CCoinBrick* coinBrick = dynamic_cast<CCoinBrick*>(e->obj);
	if (coinBrick->GetState() != BRICK_STATE_HIT)
	{
		coinBrick->SpawnCoin();
		coinBrick->SetState(BRICK_STATE_HIT);
		DebugOut(L">>> Koopa shell hit Coin Brick! >>> \n");
	}
}

void Koopas::OnCollisionWithMushroomBrick(LPCOLLISIONEVENT e)
{
	CMushroomBrick* mushroomBrick = dynamic_cast<CMushroomBrick*>(e->obj);
	if (mushroomBrick->GetState() != BRICK_STATE_HIT)
	{
		mushroomBrick->SpawnMushroom();
		mushroomBrick->SetState(BRICK_STATE_HIT);
		DebugOut(L">>> Koopa shell hit Mushroom Brick! >>> \n");
	}
}


Koopas::Koopas(float x, float y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	die_start = -1;
	shell_start = 0;
	isBeingHeld = false;
	fallSensor = new FallSensor(x, y, this);
	SetState(KOOPAS_STATE_SHELL);
	ResetSensors();
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
		isBeingHeld = false;
		if (previousState == KOOPAS_STATE_SHELL)
		{
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}
		break;

	case KOOPAS_STATE_SHELL_MOVING:
		ay = KOOPAS_GRAVITY; 
		shell_start = 0;
		isBeingHeld = false;
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

void Koopas::ChangeDirection()
{
	if (state == KOOPAS_STATE_WALKING)
	{
		vx = -vx; 
		DebugOut(L">>> Koopas is now moving in the opposite direction: %f >>> \n", vx);
		ResetSensors();
	}
}

void Koopas::ResetSensors()
{
	if (!fallSensor) return;

	float koopas_left, koopas_top, koopas_right, koopas_bottom;
	GetBoundingBox(koopas_left, koopas_top, koopas_right, koopas_bottom);

	float sensorY = koopas_bottom -5;

	if (vx > 0) 
	{
		fallSensor->SetPosition(koopas_right + 4, sensorY);
	}
	else 
	{
		fallSensor->SetPosition(koopas_left - 4, sensorY);
	}
}
