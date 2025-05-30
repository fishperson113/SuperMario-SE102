#include "Koopas.h"
#include "Goomba.h"
#include "CoinBrick.h"
#include "MushroomBrick.h"
#include "SuperLeafBrick.h"

void Koopas::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->type == KOOPAS_RED)
	{
		if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SHELL_MOVING || state == KOOPAS_STATE_UPSIDE_DOWN)
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
	else if (this->type == KOOPAS_GREEN || this->type == KOOPAS_GREEN_NO_WINGS)
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
			left = x - KOOPAS_BBOX_WIDTH / 2;
			top = y - KOOPAS_BBOX_HEIGHT / 2;
			right = left + KOOPAS_BBOX_WIDTH;
			bottom = top + KOOPAS_BBOX_HEIGHT;
		}
		else if (state == KOOPA_PARATROOPA_STATE_SHELL)
		{
			left = x - KOOPAS_BBOX_WIDTH / 2;
			top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
			right = left + KOOPAS_BBOX_WIDTH;
			bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
		}
		else if (state == KOOPAS_STATE_SHELL_MOVING)
		{
			left = x - KOOPAS_BBOX_WIDTH / 2;
			top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
			right = left + KOOPAS_BBOX_WIDTH;
			bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
		}
		else
		{
			left = x - KOOPAS_BBOX_WIDTH / 2;
			top = y - KOOPAS_BBOX_HEIGHT_SHELL / 2;
			right = left + KOOPAS_BBOX_WIDTH;
			bottom = top + KOOPAS_BBOX_HEIGHT_SHELL;
		}
	}
	else if (this->type == KOOPAS_RED_WINGS)
	{
		left = x - KOOPA_PARATROOPA_WINGS_BBOX_WIDTH / 2;
		top = y - KOOPA_PARATROOPA_WINGS_BBOX_HEIGHT / 2;
		right = left + KOOPA_PARATROOPA_WINGS_BBOX_WIDTH;
		bottom = top + KOOPA_PARATROOPA_WINGS_BBOX_HEIGHT;
	}
}

void Koopas::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->state == KOOPAS_STATE_FLOATING)
	{
		vx = 0;
		floatTime += dt * 0.001f; // Convert milliseconds to seconds

		// Calculate the new Y position using a sine wave
		float amplitude = 16.0f; // Amplitude of the floating (16 pixels)
		float frequency = 2.0f;  // Frequency of the floating (2 cycles per second)
		y = initialY + amplitude * sin(floatTime * 2 * 3.14159f * frequency);
	}
	else if (!isBeingHeld)  
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
		if (state == KOOPA_PARATROOPA_STATE_MOVING_SHELL)
		{
			if (vx > 0)
			{
				vx -= KOOPAS_SHELL_DECELERATION * dt;
				if (vx < KOOPAS_MIN_SPEED)
				{
					vx = 0;
					SetState(KOOPA_PARATROOPA_STATE_SHELL);
				}
			}
			else if (vx < 0)
			{
				vx += KOOPAS_SHELL_DECELERATION * dt;
				if (vx > -KOOPAS_MIN_SPEED)
				{
					vx = 0;
					SetState(KOOPA_PARATROOPA_STATE_SHELL);
				}
			}
		}
	}
	if (IsAboutToWakeUp())
	{
		if (this->type == KOOPAS_RED)
		{
			SetState(KOOPAS_STATE_WALKING);
		}
		else
		{
			SetState(KOOPA_PARATROOPA_STATE_WALKING);
		}
	}

	if (state == KOOPA_PARATROOPA_STATE_WALKING_WINGS && GetTickCount64() - walk_start > 1000) // Walk duration
	{
		SetState(KOOPA_PARATROOPA_STATE_JUMPING_WINGS);
	}
	else if (state == KOOPA_PARATROOPA_STATE_JUMPING_WINGS && GetTickCount64() - jump_start > 3000) // Jump duration
	{
		SetState(KOOPA_PARATROOPA_STATE_WALKING_WINGS);
	}
	else if ((state == KOOPA_PARATROOPA_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_PARATROOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	if ((state == KOOPAS_STATE_DIE) && (GetTickCount64() - die_start > KOOPA_PARATROOPA_DIE_TIMEOUT))
	{
		isDeleted = true;
		return;
	}

	/*if (!isBeingHeld && state == KOOPAS_STATE_WALKING)
	{
		if (fallSensor) fallSensor->Update(dt, coObjects);
	}*/

	if (this->vx > 0)
		this->SetDirection(1);
	else
		this->SetDirection(0);
}

void Koopas::Render()
{
	int aniId = ID_ANI_KOOPAS_WALKING_RIGHT;
	//DebugOut(L"[INFO] Koopas object type = %d!\n", this->type);
	if (this->type == KOOPAS_RED)
	{
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
			if (preState == KOOPAS_STATE_UPSIDE_DOWN)
				aniId = ID_ANI_KOOPAS_UPSIDE_DOWN;
			else
				aniId = ID_ANI_KOOPAS_SHELL;
		}
		else if (state == KOOPAS_STATE_UPSIDE_DOWN)
		{
			aniId = ID_ANI_KOOPAS_UPSIDE_DOWN;
		}
		else if (state == KOOPAS_STATE_DIE)
		{
			if (preState == KOOPAS_STATE_UPSIDE_DOWN)
				aniId = ID_ANI_KOOPAS_UPSIDE_DOWN;
			else if (preState == KOOPAS_STATE_SHELL)
				aniId = ID_ANI_KOOPAS_SHELL;
		}
	}
	else if (this->type == KOOPAS_GREEN || this->type == KOOPAS_GREEN_NO_WINGS)
	{
		if (state == KOOPA_PARATROOPA_STATE_SHELL || state == KOOPA_PARATROOPA_STATE_MOVING_SHELL)
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
		else if (state == KOOPAS_STATE_UPSIDE_DOWN)
		{
			aniId = ID_ANI_KOOPA_PARATROOPA_DIE;
		}
	}
	else if (this->type == KOOPAS_RED_WINGS)
	{
		aniId = ID_ANI_RED_WALKING_WINGS_LEFT;
		if (state == KOOPAS_STATE_WALKING)
			aniId = ID_ANI_KOOPAS_WALKING_LEFT;
		else if (state == KOOPAS_STATE_UPSIDE_DOWN)
		{
			aniId = ID_ANI_KOOPAS_UPSIDE_DOWN;
		}
	}
	
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
	RenderBoundingBox();
}

void Koopas::OnNoCollision(DWORD dt)
{
	if (!isBeingHeld && this->type != KOOPAS_RED_WINGS)  
	{
		x += vx * dt;
		y += vy * dt;
	}
	if (this->type == KOOPAS_RED_WINGS && this->state == KOOPAS_STATE_WALKING)
		y += vy * dt;
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

	if (state != KOOPAS_STATE_SHELL_MOVING && state != KOOPA_PARATROOPA_STATE_MOVING_SHELL)
		return;

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<Koopas*>(e->obj))
		OnCollisionWithOtherKoopas(e);
	else if (dynamic_cast<CCoinBrick*>(e->obj))
		OnCollisionWithCoinBrick(e);
	else if (dynamic_cast<CMushroomBrick*>(e->obj))
		OnCollisionWithMushroomBrick(e);
	else if (dynamic_cast<CSuperLeafBrick*>(e->obj))
		OnCollisionWithLeafBrick(e);
	else if (dynamic_cast<CBrick*>(e->obj))
		OnCollisionWithBrick(e);
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
		otherKoopas->GetState() != KOOPAS_STATE_SHELL_MOVING &&
		otherKoopas->GetState() != KOOPA_PARATROOPA_STATE_SHELL &&
		otherKoopas->GetState() != KOOPA_PARATROOPA_STATE_MOVING_SHELL)
	{
		if (otherKoopas->GetType() == KOOPAS_RED || otherKoopas->GetType() == KOOPAS_RED_WINGS)
		{
			otherKoopas->SetState(KOOPAS_STATE_SHELL);
		}
		else if (otherKoopas->GetType() == KOOPAS_GREEN || otherKoopas->GetType() == KOOPAS_GREEN_NO_WINGS)
		{
			otherKoopas->SetState(KOOPA_PARATROOPA_STATE_SHELL);
		}
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

void Koopas::OnCollisionWithLeafBrick(LPCOLLISIONEVENT e)
{
	CSuperLeafBrick* leafBrick = dynamic_cast<CSuperLeafBrick*>(e->obj);
	if (leafBrick->GetState() != BRICK_STATE_HIT)
	{
		leafBrick->SpawnSuperLeaf();
		leafBrick->SetState(BRICK_STATE_HIT);
		DebugOut(L">>> Koopa shell hit Leaf Brick! >>> \n");
	}
}

void Koopas::OnCollisionWithBrick(LPCOLLISIONEVENT e)
{
	CBrick* brick = dynamic_cast<CBrick*>(e->obj);

	if (e->nx != 0)	//collision from left and right
	{
		if (brick->IsBreakable())
		{
			brick->Break();
		}
	}
}


Koopas::Koopas(float x, float y, int type) :CGameObject(x, y)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	this->type = type;
	die_start = -1;
	shell_start = 0;
	isBeingHeld = false;
	fallSensor = nullptr;
	DebugOut(L"[INFO] Koopas object type = %d!\n", this->type);
	if (this->type == KOOPAS_RED)
	{
		fallSensor = new FallSensor(x, y, this);
		SetState(KOOPAS_STATE_WALKING);
	}
	else if (this->type == KOOPAS_GREEN)
	{
		SetState(KOOPA_PARATROOPA_STATE_WALKING);
	}
	else if (this->type == KOOPAS_GREEN_NO_WINGS)
	{
		SetState(KOOPA_PARATROOPA_STATE_WALKING_WINGS);
	}
	else if (this->type == KOOPAS_RED_WINGS)
	{
		SetState(KOOPAS_STATE_FLOATING);
		this->initialY = y;
		this->floatTime = 0.0f;
	}
	DebugOut(L"[INFO] Koopas object type = %d!\n", this->type);
	ResetSensors();
}

void Koopas::SetState(int state)
{
	int previousState = this->state;
	this->preState = this->state;
	this->state = state;
	switch (state)
	{
	case KOOPA_PARATROOPA_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		isBeingHeld = false;
		if (previousState == KOOPA_PARATROOPA_STATE_SHELL)
		{
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		ay = KOOPAS_GRAVITY;
		isBeingHeld = false;
		if (previousState == KOOPAS_STATE_SHELL)
		{
			y -= (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}
		break;
	case KOOPA_PARATROOPA_STATE_MOVING_SHELL:
	case KOOPAS_STATE_SHELL_MOVING:
		ay = KOOPAS_GRAVITY; 
		shell_start = 0;
		isBeingHeld = false;
		break;
	case KOOPA_PARATROOPA_STATE_SHELL:
		vx = 0;
		ay = KOOPAS_GRAVITY;

		if (previousState == KOOPA_PARATROOPA_STATE_WALKING)
		{
			y += (KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_SHELL) / 2;
		}

		shell_start = GetTickCount64();
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
		ay = KOOPAS_GRAVITY;
		isBeingHeld = false;
		break;
	case KOOPA_PARATROOPA_STATE_JUMPING_WINGS:
		vy = KOOPA_PARATROOPA_JUMP_SPEED;
		jump_start = GetTickCount64();
		break;
	case KOOPAS_STATE_FLOATING:
		walk_start = GetTickCount64();
		break;
	case KOOPAS_STATE_DIE:
		die_start = GetTickCount64();
		vx = 0;
		vy = 0;
		ay = 0;
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
