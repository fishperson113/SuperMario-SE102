#include <algorithm>
#include "debug.h"

#include "Mario.h"
#include "Game.h"

#include "Goomba.h"
#include "Coin.h"
#include "Portal.h"
#include "CoinBrick.h"
#include "Mushroom.h"
#include "MushroomBrick.h"
#include "Koopas.h"
#include "Collision.h"

void CMario::HoldKoopas(Koopas* koopas)
{
	if (isHolding) return; 

	heldKoopas = koopas;
	isHolding = true;

	if (heldKoopas->GetState() != KOOPAS_STATE_SHELL)
		heldKoopas->SetState(KOOPAS_STATE_SHELL);

	heldKoopas->SetBeingHeld(true);

	UpdateHeldKoopasPosition();
}

void CMario::ReleaseKoopas()
{
	if (!isHolding || heldKoopas == NULL) return;

	isKicking = true;
	kick_start = GetTickCount64();

	heldKoopas->SetBeingHeld(false);
	heldKoopas->SetState(KOOPAS_STATE_SHELL_MOVING);
	heldKoopas->SetSpeed(nx * KOOPAS_SHELL_SPEED, 0);

	isHolding = false;
	heldKoopas = NULL;

	DebugOut(L">>> Mario released Koopa shell! >>> \n");
}

void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	if (abs(vx) > abs(maxVx)) vx = maxVx;

	if ( GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME) 
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (isKicking && GetTickCount64() - kick_start > MARIO_KICK_TIMEOUT)
	{
		isKicking = false;
	}

	if (isHolding && heldKoopas)
	{
		if (heldKoopas->IsAboutToWakeUp())
		{
			isHolding = false;
			heldKoopas = NULL;
			if (level > MARIO_LEVEL_SMALL)
			{
				level = MARIO_LEVEL_SMALL;
				StartUntouchable();
			}
			else
			{
				SetState(MARIO_STATE_DIE);
			}
		}
		else
		{
			UpdateHeldKoopasPosition();
		}
	}

	if (isHolding && !CGame::GetInstance()->IsKeyDown(DIK_A))
	{
		ReleaseKoopas();
	}
}

void CMario::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
	isOnPlatform = false;
}

void CMario::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->ny != 0 && e->obj->IsBlocking())
	{
		vy = 0;
		if (e->ny < 0) isOnPlatform = true;
	}
	else 
	if (e->nx != 0 && e->obj->IsBlocking())
	{
		vx = 0;
	}

	if (IsGodMode())
	{
		if (dynamic_cast<CGoomba*>(e->obj))
		{
			CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				goomba->SetState(GOOMBA_STATE_DIE);
				return; 
			}
		}
		else if (dynamic_cast<Koopas*>(e->obj))
		{
			Koopas* koopas = dynamic_cast<Koopas*>(e->obj);
			if (koopas->GetState() != KOOPAS_STATE_SHELL)
			{
				koopas->SetState(KOOPAS_STATE_SHELL);
				return; 
			}
		}
	}

	if (dynamic_cast<CGoomba*>(e->obj))
		OnCollisionWithGoomba(e);
	else if (dynamic_cast<CCoin*>(e->obj))
		OnCollisionWithCoin(e);
	else if (dynamic_cast<CPortal*>(e->obj))
		OnCollisionWithPortal(e);
	else if (dynamic_cast<CCoinBrick*>(e->obj))
		OnCollisionWithCoinBrick(e);
	else if (dynamic_cast<CMushroom*>(e->obj))
		OnCollisionWithMushroom(e);
	else if (dynamic_cast<CMushroomBrick*>(e->obj))
		OnCollisionWithMushroomBrick(e);
	else if (dynamic_cast<Koopas*>(e->obj))
		OnCollisionWithKoopas(e);
}

void CMario::UpdateHeldKoopasPosition()
{
	if (!isHolding || heldKoopas == NULL) return;

	float holdOffsetX = (nx > 0) ? 10.0f : -10.0f;
	float holdOffsetY = -7.0f;

	heldKoopas->SetPosition(x + holdOffsetX, y + holdOffsetY);
}

void CMario::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj);

	// jump on top >> kill Goomba and deflect a bit 
	if (e->ny < 0)
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (goomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					level = MARIO_LEVEL_SMALL;
					StartUntouchable();
				}
				else
				{
					DebugOut(L">>> Mario DIE >>> \n");
					SetState(MARIO_STATE_DIE);
				}
			}
		}
	}
}

void CMario::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	coin++;
}

void CMario::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* p = (CPortal*)e->obj;
	CGame::GetInstance()->InitiateSwitchScene(p->GetSceneId());
}

void CMario::OnCollisionWithCoinBrick(LPCOLLISIONEVENT e)
{
	CCoinBrick* coinBrick = dynamic_cast<CCoinBrick*>(e->obj);

	if (e->ny > 0)	//collision from below
	{
		if (coinBrick->GetState() == BRICK_STATE_HIT) return; // already hit
		coinBrick->SpawnCoin();
		coinBrick->SetState(BRICK_STATE_HIT);
	}
}

void CMario::OnCollisionWithMushroom(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	this->SetLevel(MARIO_LEVEL_BIG);
}

void CMario::OnCollisionWithMushroomBrick(LPCOLLISIONEVENT e)
{
	CMushroomBrick* mushroomBrick = dynamic_cast<CMushroomBrick*>(e->obj);

	if (e->ny > 0)	//collision from below
	{
		if (mushroomBrick->GetState() == BRICK_STATE_HIT) return; // already hit
		mushroomBrick->SpawnMushroom();
		mushroomBrick->SetState(BRICK_STATE_HIT);
	}
}

void CMario::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	Koopas* koopas = (Koopas*)e->obj;

	// Mario jumps on Koopas from above
	if (e->ny < 0)
	{
		if (koopas->GetState() == KOOPAS_STATE_WALKING)
		{
			// Turn normal Koopas into shell
			koopas->SetState(KOOPAS_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL)
		{
			// If Koopas is already in shell state, kick it
			float shellDirection;

			// Determine kick direction based on Mario's direction
			if (nx > 0)
				shellDirection = 1.0f; // Kick to the right
			else
				shellDirection = -1.0f; // Kick to the left

			koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
			koopas->SetSpeed(shellDirection * KOOPAS_SHELL_SPEED, 0);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			DebugOut(L">>> Mario kicked Koopa shell by jumping on it! >>> \n");
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING)
		{
			// If shell is already moving, stop it
			koopas->SetState(KOOPAS_STATE_SHELL);
			vy = -MARIO_JUMP_DEFLECT_SPEED;
			DebugOut(L">>> Mario stopped moving Koopa shell! >>> \n");
		}
	}
	else if (e->nx != 0) // Side collision
	{
		// Check if we can hold the Koopa shell (only when running with A pressed)
		if (koopas->GetState() == KOOPAS_STATE_SHELL &&
			(abs(vx) >= MARIO_WALKING_SPEED) &&
			CGame::GetInstance()->IsKeyDown(DIK_A) &&
			!isHolding)
		{
			HoldKoopas(koopas);
			DebugOut(L">>> Mario picked up Koopa shell! >>> \n");
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL)
		{
			isKicking = true;
			kick_start = GetTickCount64();

			int shellDirection = (e->nx > 0) ? -1 : 1; // Determine direction based on collision
			koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
			koopas->SetSpeed(shellDirection * KOOPAS_SHELL_SPEED, 0);
			DebugOut(L">>> Mario kicked Koopa shell from the side! >>> \n");
		}
		else // hit by Koopas
		{
			if (untouchable == 0)
			{
				if (koopas->GetState() != KOOPAS_STATE_SHELL)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						level = MARIO_LEVEL_SMALL;
						StartUntouchable();
					}
					else
					{
						DebugOut(L">>> Mario DIE >>> \n");
						SetState(MARIO_STATE_DIE);
					}
				}
			}
		}
	}
}

//
// Get animation ID for small Mario
//
int CMario::GetAniIdSmall()
{
	int aniId = -1;

	// Check if Mario is kicking first
	if (isKicking)
	{
		if (nx >= 0)
			return ID_ANI_MARIO_SMALL_KICK_RIGHT;
		else
			return ID_ANI_MARIO_SMALL_KICK_LEFT;
	}

	if (isHolding)
	{
		if (!isOnPlatform)
		{
			// Jumping while holding
			if (abs(ax) == MARIO_ACCEL_RUN_X)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
			}
		}
		else if (vx == 0)
		{
			// Idle while holding
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT;
		}
		else if (vx > 0)
		{
			// Running while holding, facing right
			aniId = ID_ANI_MARIO_SMALL_HOLD_RUNNING_RIGHT;
		}
		else // vx < 0
		{
			// Running while holding, facing left
			aniId = ID_ANI_MARIO_SMALL_HOLD_RUNNING_LEFT;
		}

		if (aniId != -1) return aniId;
	}


	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
		}
	}
	else if (isSitting)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_SIT_RIGHT;
		else
			aniId = ID_ANI_MARIO_SIT_LEFT;
	}
	else if (vx == 0)
	{
		if (nx > 0)
			aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;
		else
			aniId = ID_ANI_MARIO_SMALL_IDLE_LEFT;
	}
	else if (vx > 0)
	{
		if (ax < 0)
			aniId = ID_ANI_MARIO_SMALL_BRACE_RIGHT;
		else if (ax == MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
		else if (ax == MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_SMALL_WALKING_RIGHT;
	}
	else // vx < 0
	{
		if (ax > 0)
			aniId = ID_ANI_MARIO_SMALL_BRACE_LEFT;
		else if (ax == -MARIO_ACCEL_RUN_X)
			aniId = ID_ANI_MARIO_SMALL_RUNNING_LEFT;
		else if (ax == -MARIO_ACCEL_WALK_X)
			aniId = ID_ANI_MARIO_SMALL_WALKING_LEFT;
	}

	if (aniId == -1) aniId = ID_ANI_MARIO_SMALL_IDLE_RIGHT;

	return aniId;
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	int aniId = -1;

	if (isKicking)
	{
		if (nx >= 0)
			return ID_ANI_MARIO_KICK_RIGHT;
		else
			return ID_ANI_MARIO_KICK_LEFT;
	}

	if (isHolding)
	{
		// Handle holding animations for all scenarios
		if (!isOnPlatform)
		{
			// Jumping while holding
			if (abs(ax) == MARIO_ACCEL_RUN_X)
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
			}
			else
			{
				if (nx >= 0)
					aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
				else
					aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
			}
		}
		else if (vx == 0)
		{
			// Idle while holding
			if (nx >= 0)
				aniId = ID_ANI_MARIO_HOLD_IDLE_RIGHT;
			else
				aniId = ID_ANI_MARIO_HOLD_IDLE_LEFT;
		}
		else if (vx > 0)
		{
			aniId = ID_ANI_MARIO_HOLD_RUNNING_RIGHT;
		}
		else // vx < 0
		{
			aniId = ID_ANI_MARIO_HOLD_RUNNING_LEFT;
		}

		if (aniId != -1) return aniId;
	}

	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_RUN_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_RUN_LEFT;
		}
		else
		{
			if (nx >= 0)
				aniId = ID_ANI_MARIO_JUMP_WALK_RIGHT;
			else
				aniId = ID_ANI_MARIO_JUMP_WALK_LEFT;
		}
	}
	else
		if (isSitting)
		{
			if (nx > 0)
				aniId = ID_ANI_MARIO_SIT_RIGHT;
			else
				aniId = ID_ANI_MARIO_SIT_LEFT;
		}
		else
			if (vx == 0)
			{
				if (nx > 0) aniId = ID_ANI_MARIO_IDLE_RIGHT;
				else aniId = ID_ANI_MARIO_IDLE_LEFT;
			}
			else if (vx > 0)
			{
				if (ax < 0)
					aniId = ID_ANI_MARIO_BRACE_RIGHT;
				else if (ax == MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_RIGHT;
				else if (ax == MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_RIGHT;
			}
			else // vx < 0
			{
				if (ax > 0)
					aniId = ID_ANI_MARIO_BRACE_LEFT;
				else if (ax == -MARIO_ACCEL_RUN_X)
					aniId = ID_ANI_MARIO_RUNNING_LEFT;
				else if (ax == -MARIO_ACCEL_WALK_X)
					aniId = ID_ANI_MARIO_WALKING_LEFT;
			}

	if (aniId == -1) aniId = ID_ANI_MARIO_IDLE_RIGHT;

	return aniId;
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_BIG)
		aniId = GetAniIdBig();
	else if (level == MARIO_LEVEL_SMALL)
		aniId = GetAniIdSmall();

	animations->Get(aniId)->Render(x, y);

	//RenderBoundingBox();
	
	DebugOutTitle(L"Coins: %d", coin);
}

void CMario::SetState(int state)
{
	// DIE is the end state, cannot be changed! 
	if (this->state == MARIO_STATE_DIE) return; 

	switch (state)
	{
	case MARIO_STATE_RUNNING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		maxVx = -MARIO_WALKING_SPEED;
		ax = -MARIO_ACCEL_WALK_X;
		nx = -1;
		break;
	case MARIO_STATE_JUMP:
		if (isSitting) break;
		if (isOnPlatform)
		{
			if (abs(this->vx) == MARIO_RUNNING_SPEED)
				vy = -MARIO_JUMP_RUN_SPEED_Y;
			else
				vy = -MARIO_JUMP_SPEED_Y;
		}
		break;

	case MARIO_STATE_RELEASE_JUMP:
		if (vy < 0) vy += MARIO_JUMP_SPEED_Y / 2;
		break;

	case MARIO_STATE_SIT:
		if (isHolding) // Can't sit while holding a shell
		{
			ReleaseKoopas();
		}
		if (isOnPlatform && level != MARIO_LEVEL_SMALL)
		{
			state = MARIO_STATE_IDLE;
			isSitting = true;
			vx = 0; vy = 0.0f;
			y +=MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_SIT_RELEASE:
		if (isSitting)
		{
			isSitting = false;
			state = MARIO_STATE_IDLE;
			y -= MARIO_SIT_HEIGHT_ADJUST;
		}
		break;

	case MARIO_STATE_IDLE:
		ax = 0.0f;
		vx = 0.0f;
		break;

	case MARIO_STATE_DIE:
		vy = -MARIO_JUMP_DEFLECT_SPEED;
		vx = 0;
		ax = 0;
		break;
	}

	CGameObject::SetState(state);
}

bool CMario::IsGodMode()
{
	return isHolding && heldKoopas && !heldKoopas->IsAboutToWakeUp();
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG)
	{
		if (isSitting)
		{
			left = x - MARIO_BIG_SITTING_BBOX_WIDTH / 2;
			top = y - MARIO_BIG_SITTING_BBOX_HEIGHT / 2;
			right = left + MARIO_BIG_SITTING_BBOX_WIDTH;
			bottom = top + MARIO_BIG_SITTING_BBOX_HEIGHT;
		}
		else 
		{
			left = x - MARIO_BIG_BBOX_WIDTH/2;
			top = y - MARIO_BIG_BBOX_HEIGHT/2;
			right = left + MARIO_BIG_BBOX_WIDTH;
			bottom = top + MARIO_BIG_BBOX_HEIGHT;
		}
	}
	else
	{
		left = x - MARIO_SMALL_BBOX_WIDTH/2;
		top = y - MARIO_SMALL_BBOX_HEIGHT/2;
		right = left + MARIO_SMALL_BBOX_WIDTH;
		bottom = top + MARIO_SMALL_BBOX_HEIGHT;
	}
}

void CMario::SetLevel(int l)
{
	// Adjust position to avoid falling off platform
	if (this->level == MARIO_LEVEL_SMALL)
	{
		y -= (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
	}
	level = l;
}

