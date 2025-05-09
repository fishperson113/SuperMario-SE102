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
#include "FallPitch.h"
#include "ParaGoomba.h"
#include "PiranhaPlant.h"
#include "SuperLeaf.h"
#include "SuperLeafBrick.h"
#include "Bullet.h"

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

	// Update velocity based on acceleration
	UpdateVelocity(dt);

	// Apply velocity limits
	if (abs(vx) > abs(maxVx)) vx = maxVx;

	// Handle untouchable state
	UpdateUntouchableState();

	// Handle kicking timeout
	UpdateKickingState();

	// Update power meter
	UpdatePowerMeter(dt);

	// Handle shell holding
	UpdateHeldKoopas();

	// Handle spinning state
	UpdateSpinningState();

	UpdateFlyingState();

	// Handle gliding state
	if(!isFlying)
		UpdateGlidingState();
	// Debug output
	DebugOut(L"Coins: %d | P-Meter: %.2f/%.0f", coin, powerMeter, MARIO_PMETER_MAX);
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
		HandleEnemyCollisionsInGodMode(e);
		return;
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
	else if (dynamic_cast<CFallPitch*>(e->obj))
		OnCollisionWithFallPitch(e);
	else if (dynamic_cast<CParaGoomba*>(e->obj))
		OnCollisionWithParaGoomba(e);
	else if (dynamic_cast<CPiranhaPlant*>(e->obj))
		OnCollisionWithPiranhaPlant(e);
	else if (dynamic_cast<CSuperLeaf*>(e->obj))
		OnCollisionWithSuperLeaf(e);
	else if (dynamic_cast<CSuperLeafBrick*>(e->obj))
		OnCollisionWithSuperLeafBrick(e);
	else if (dynamic_cast<Checkpoint*>(e->obj))
		OnCollisionWithCheckpoint(e);
	else if (dynamic_cast<CBullet*>(e->obj))
		OnCollisionWithBullet(e);
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
					LevelDown();
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

void CMario::OnCollisionWithFallPitch(LPCOLLISIONEVENT e)
{
	this->SetState(MARIO_STATE_DIE);
	DebugOut(L">>> Mario DIE >>> \n");
}

void CMario::OnCollisionWithParaGoomba(LPCOLLISIONEVENT e)
{
	CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj);
	if (e->ny < 0)
	{
		if (paraGoomba->GetState() == PARAGOOMBA_STATE_WALKING || paraGoomba->GetState() == PARAGOOMBA_STATE_JUMPING)
			paraGoomba->SetState(PARAGOOMBA_STATE_WALKING2);
		else if (paraGoomba->GetState() == PARAGOOMBA_STATE_WALKING2)
			paraGoomba->SetState(PARAGOOMBA_STATE_DIE);
		vy = -MARIO_JUMP_DEFLECT_SPEED;
	}
	else // hit by Goomba
	{
		if (untouchable == 0)
		{
			if (paraGoomba->GetState() != GOOMBA_STATE_DIE)
			{
				if (level > MARIO_LEVEL_SMALL)
				{
					LevelDown();
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

void CMario::OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e)
{
	if (this->untouchable == 0)
	{
		if (e->ny < 0)
			vy = -MARIO_JUMP_DEFLECT_SPEED;
		if (level > MARIO_LEVEL_SMALL)
		{
			LevelDown();
			StartUntouchable();
		}
		else
		{
			DebugOut(L">>> Mario DIE >>> \n");
			SetState(MARIO_STATE_DIE);
		}
	}
}

void CMario::OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e)
{
	e->obj->Delete();
	if (level == MARIO_LEVEL_SMALL)
	{
		this->SetLevel(MARIO_LEVEL_BIG);
	}
	else {
		this->SetLevel(MARIO_LEVEL_TAIL);
	}
}

void CMario::OnCollisionWithSuperLeafBrick(LPCOLLISIONEVENT e)
{
	CSuperLeafBrick* superLeafBrick = dynamic_cast<CSuperLeafBrick*>(e->obj);

	if (e->ny > 0)	//collision from below
	{
		if (superLeafBrick->GetState() == BRICK_STATE_HIT) return; // already hit
		superLeafBrick->SpawnSuperLeaf ();
		superLeafBrick->SetState(BRICK_STATE_HIT);
	}
}

void CMario::OnCollisionWithCheckpoint(LPCOLLISIONEVENT e)
{
	Checkpoint* checkpoint = dynamic_cast<Checkpoint*>(e->obj);

	if (checkpoint && !checkpoint->IsActivated())
	{
		checkpoint->SetState(CHECKPOINT_STATE_ACTIVE);
		DebugOut(L">>> Mario triggered a checkpoint! >>> \n");
	}
}

void CMario::OnCollisionWithBullet(LPCOLLISIONEVENT e)
{
	if (this->untouchable == 0)
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
			koopas->SetSpeed(0, 0);
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
						LevelDown();
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
	if (isKicking)
	return (nx >= 0) ? ID_ANI_MARIO_SMALL_KICK_RIGHT : ID_ANI_MARIO_SMALL_KICK_LEFT;

	// Handle holding animations
	if (isHolding)
	{
		int aniId = GetSmallHoldingAniId();
		if (aniId != -1) return aniId;
	}

	// Handle jumping animations
	if (!isOnPlatform)
	return GetSmallJumpingAniId();

	// Handle sitting
	if (isSitting)
	return (nx > 0) ? ID_ANI_MARIO_SIT_RIGHT : ID_ANI_MARIO_SIT_LEFT;

	// Handle idle
	if (vx == 0)
	return (nx > 0) ? ID_ANI_MARIO_SMALL_IDLE_RIGHT : ID_ANI_MARIO_SMALL_IDLE_LEFT;

	// Handle moving right
	if (vx > 0)
	return GetSmallRightMovingAniId();

	// Handle moving left
	return GetSmallLeftMovingAniId();

}

int CMario::GetAniIdTail()
{
	// Handle spin attack animation first
	if (isSpinning)
		return ID_ANI_MARIO_TAIL_SPIN_ATTACK;

	if (isKicking)
		return (nx >= 0) ? ID_ANI_MARIO_TAIL_KICK_RIGHT : ID_ANI_MARIO_TAIL_KICK_LEFT;

	// Handle holding animations
	if (isHolding)
	{
		int aniId = GetTailHoldingAniId();
		if (aniId != -1) return aniId;
	}
	// Handle gliding animations
	if (isGliding)
	{
		if (nx > 0) return ID_ANI_MARIO_TAIL_GLIDE_RIGHT;
		else return ID_ANI_MARIO_TAIL_GLIDE_LEFT;
	}

	if (isFlying)
	{
		return (nx > 0) ? ID_ANI_MARIO_TAIL_FLY_RIGHT : ID_ANI_MARIO_TAIL_FLY_LEFT;
	}

	// Handle jumping animations
	if (!isOnPlatform)
		return GetTailJumpingAniId();

	// Handle sitting
	if (isSitting)
		return (nx > 0) ? ID_ANI_MARIO_TAIL_SIT_RIGHT : ID_ANI_MARIO_TAIL_SIT_LEFT;

	// Handle idle
	if (vx == 0)
		return (nx > 0) ? ID_ANI_MARIO_TAIL_IDLE_RIGHT : ID_ANI_MARIO_TAIL_IDLE_LEFT;

	// Handle moving right
	if (vx > 0)
		return GetTailRightMovingAniId();

	// Handle moving left
	return GetTailLeftMovingAniId();
}

int CMario::GetSmallHoldingAniId()
{
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
			return (nx >= 0) ? ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT : ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
		else
			return (nx >= 0) ? ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT : ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
	}

	if (vx == 0)
		return (nx >= 0) ? ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT : ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT;

	if (vx > 0)
		return ID_ANI_MARIO_SMALL_HOLD_RUNNING_RIGHT;
	else // vx < 0
		return ID_ANI_MARIO_SMALL_HOLD_RUNNING_LEFT;

	return -1; 
}

int CMario::GetSmallJumpingAniId()
{
	if (abs(ax) == MARIO_ACCEL_RUN_X)
		return (nx >= 0) ? ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT : ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT;
	else
		return (nx >= 0) ? ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT : ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT;
}

int CMario::GetSmallRightMovingAniId()
{
	if (ax < 0)
		return ID_ANI_MARIO_SMALL_BRACE_RIGHT;
	else if (ax == MARIO_ACCEL_RUN_X)
		return ID_ANI_MARIO_SMALL_RUNNING_RIGHT;
	else if (ax == MARIO_ACCEL_WALK_X)
		return ID_ANI_MARIO_SMALL_WALKING_RIGHT;

	return ID_ANI_MARIO_SMALL_IDLE_RIGHT; // Default
}

int CMario::GetSmallLeftMovingAniId()
{
	if (ax > 0)
		return ID_ANI_MARIO_SMALL_BRACE_LEFT;
	else if (ax == -MARIO_ACCEL_RUN_X)
		return ID_ANI_MARIO_SMALL_RUNNING_LEFT;
	else if (ax == -MARIO_ACCEL_WALK_X)
		return ID_ANI_MARIO_SMALL_WALKING_LEFT;

	return ID_ANI_MARIO_SMALL_IDLE_LEFT; // Default
}

int CMario::GetBigHoldingAniId()
{
	// Not on platform, jumping while holding
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
			return (nx >= 0) ? ID_ANI_MARIO_JUMP_RUN_RIGHT : ID_ANI_MARIO_JUMP_RUN_LEFT;
		else
			return (nx >= 0) ? ID_ANI_MARIO_JUMP_WALK_RIGHT : ID_ANI_MARIO_JUMP_WALK_LEFT;
	}

	// Idle while holding
	if (vx == 0)
		return (nx >= 0) ? ID_ANI_MARIO_HOLD_IDLE_RIGHT : ID_ANI_MARIO_HOLD_IDLE_LEFT;

	// Running while holding
	if (vx > 0)
		return ID_ANI_MARIO_HOLD_RUNNING_RIGHT;
	else // vx < 0
		return ID_ANI_MARIO_HOLD_RUNNING_LEFT;

	return -1; // No applicable animation found
}

int CMario::GetBigJumpingAniId()
{
	if (abs(ax) == MARIO_ACCEL_RUN_X)
		return (nx >= 0) ? ID_ANI_MARIO_JUMP_RUN_RIGHT : ID_ANI_MARIO_JUMP_RUN_LEFT;
	else
		return (nx >= 0) ? ID_ANI_MARIO_JUMP_WALK_RIGHT : ID_ANI_MARIO_JUMP_WALK_LEFT;
}

int CMario::GetBigRightMovingAniId()
{
	if (ax < 0)
		return ID_ANI_MARIO_BRACE_RIGHT;
	else if (ax == MARIO_ACCEL_RUN_X)
		return ID_ANI_MARIO_RUNNING_RIGHT;
	else if (ax == MARIO_ACCEL_WALK_X)
		return ID_ANI_MARIO_WALKING_RIGHT;

	return ID_ANI_MARIO_IDLE_RIGHT; // Default
}

int CMario::GetBigLeftMovingAniId()
{
	if (ax > 0)
		return ID_ANI_MARIO_BRACE_LEFT;
	else if (ax == -MARIO_ACCEL_RUN_X)
		return ID_ANI_MARIO_RUNNING_LEFT;
	else if (ax == -MARIO_ACCEL_WALK_X)
		return ID_ANI_MARIO_WALKING_LEFT;

	return ID_ANI_MARIO_IDLE_LEFT; // Default
}

int CMario::GetTailHoldingAniId()
{
	// Not on platform, jumping while holding
	if (!isOnPlatform)
	{
		if (abs(ax) == MARIO_ACCEL_RUN_X)
			return (nx >= 0) ? ID_ANI_MARIO_TAIL_JUMP_RUN_RIGHT : ID_ANI_MARIO_TAIL_JUMP_RUN_LEFT;
		else
			return (nx >= 0) ? ID_ANI_MARIO_TAIL_JUMP_WALK_RIGHT : ID_ANI_MARIO_TAIL_JUMP_WALK_LEFT;
	}

	// Idle while holding
	if (vx == 0)
		return (nx >= 0) ? ID_ANI_MARIO_TAIL_HOLD_IDLE_RIGHT : ID_ANI_MARIO_TAIL_HOLD_IDLE_LEFT;

	// Running while holding
	if (vx > 0)
		return ID_ANI_MARIO_TAIL_HOLD_RUNNING_RIGHT;
	else // vx < 0
		return ID_ANI_MARIO_TAIL_HOLD_RUNNING_LEFT;

	return -1; // No applicable animation found
}

int CMario::GetTailJumpingAniId()
{
	if (abs(ax) == MARIO_ACCEL_RUN_X)
		return (nx >= 0) ? ID_ANI_MARIO_TAIL_JUMP_RUN_RIGHT : ID_ANI_MARIO_TAIL_JUMP_RUN_LEFT;
	else
		return (nx >= 0) ? ID_ANI_MARIO_TAIL_JUMP_WALK_RIGHT : ID_ANI_MARIO_TAIL_JUMP_WALK_LEFT;
}

int CMario::GetTailRightMovingAniId()
{
	if (ax < 0)
		return ID_ANI_MARIO_TAIL_BRACE_RIGHT;
	else if (ax == MARIO_ACCEL_RUN_X)
		return ID_ANI_MARIO_TAIL_RUNNING_RIGHT;
	else if (ax == MARIO_ACCEL_WALK_X)
		return ID_ANI_MARIO_TAIL_WALKING_RIGHT;

	return ID_ANI_MARIO_TAIL_IDLE_RIGHT; // Default
}

int CMario::GetTailLeftMovingAniId()
{
	if (ax > 0)
		return ID_ANI_MARIO_TAIL_BRACE_LEFT;
	else if (ax == -MARIO_ACCEL_RUN_X)
		return ID_ANI_MARIO_TAIL_RUNNING_LEFT;
	else if (ax == -MARIO_ACCEL_WALK_X)
		return ID_ANI_MARIO_TAIL_WALKING_LEFT;

	return ID_ANI_MARIO_TAIL_IDLE_LEFT; // Default
}

void CMario::HandleKoopasJumpedOn(Koopas* koopas)
{
   vy = -MARIO_JUMP_DEFLECT_SPEED;

   switch (koopas->GetState())
   {
   case KOOPAS_STATE_WALKING:
       // Turn normal Koopas into shell
       koopas->SetState(KOOPAS_STATE_SHELL);
       break;

   case KOOPAS_STATE_SHELL:
   {
       // If Koopas is already in shell state, kick it
       float shellDirection = (nx > 0) ? 1.0f : -1.0f;
       koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
       koopas->SetSpeed(shellDirection * KOOPAS_SHELL_SPEED, 0);
       DebugOut(L">>> Mario kicked Koopa shell by jumping on it! >>> \n");
       break;
   }

   case KOOPAS_STATE_SHELL_MOVING:
       // If shell is already moving, stop it
       koopas->SetState(KOOPAS_STATE_SHELL);
       koopas->SetSpeed(0, 0);
       DebugOut(L">>> Mario stopped moving Koopa shell! >>> \n");
       break;
   }
}

void CMario::HandleKoopasSideCollision(Koopas* koopas, LPCOLLISIONEVENT e)
{
	// Can we hold the shell?
	if (koopas->GetState() == KOOPAS_STATE_SHELL && abs(vx) >= MARIO_WALKING_SPEED &&CGame::GetInstance()->IsKeyDown(DIK_A) &&
		!isHolding)
	{
		HoldKoopas(koopas);
		DebugOut(L">>> Mario picked up Koopa shell! >>> \n");
		return;
	}

	// Can we kick the shell?
	if (koopas->GetState() == KOOPAS_STATE_SHELL)
	{
		KickKoopasShell(koopas, e);
		return;
	}

	// Handle damage when hit by Koopas
	if (untouchable == 0 && koopas->GetState() != KOOPAS_STATE_SHELL)
		TakeDamage();
}

void CMario::KickKoopasShell(Koopas* koopas, LPCOLLISIONEVENT e)
{
	isKicking = true;
	kick_start = GetTickCount64();

	int shellDirection = (e->nx > 0) ? -1 : 1;
	koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
	koopas->SetSpeed(shellDirection * KOOPAS_SHELL_SPEED, 0);
	DebugOut(L">>> Mario kicked Koopa shell from the side! >>> \n");
}

void CMario::TakeDamage()
{
	if (level > MARIO_LEVEL_SMALL)
	{
		LevelDown();
		StartUntouchable();
	}
	else
	{
		DebugOut(L">>> Mario DIE >>> \n");
		SetState(MARIO_STATE_DIE);
	}
}

void CMario::LevelDown()
{
	// If already at the lowest level, do nothing
	if (level <= MARIO_LEVEL_SMALL)
		return;

	StartUntouchable();

	if (level == MARIO_LEVEL_TAIL)
	{
		level = MARIO_LEVEL_BIG;
		DebugOut(L">>> Mario lost tail: reverted to Big Mario >>> \n");
	}
	else if (level == MARIO_LEVEL_BIG)
	{
		level = MARIO_LEVEL_SMALL;
		y += (MARIO_BIG_BBOX_HEIGHT - MARIO_SMALL_BBOX_HEIGHT) / 2;
		DebugOut(L">>> Mario lost power: reverted to Small Mario >>> \n");
	}
}

void CMario::UpdateVelocity(DWORD dt)
{
	vx += ax * dt;
}

void CMario::UpdateUntouchableState()
{
	if (GetTickCount64() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}
}

void CMario::UpdateKickingState()
{
	if (isKicking && GetTickCount64() - kick_start > MARIO_KICK_TIMEOUT)
	{
		isKicking = false;
	}
}

void CMario::UpdateSpinningState()
{
	if (isSpinning && GetTickCount64() - spin_start > MARIO_SPIN_ATTACK_TIMEOUT)
	{
		isSpinning = false;
		DebugOut(L">>> Mario spin attack ended! >>> \n");

		spin_cooldown_start = GetTickCount64();
		canSpin = false;
	}

	if (!canSpin && GetTickCount64() - spin_cooldown_start > MARIO_SPIN_ATTACK_COOLDOWN)
	{
		canSpin = true;
		DebugOut(L">>> Mario can spin again! >>> \n");
	}
}
void CMario::StartSpinAttack()
{
	if (level == MARIO_LEVEL_TAIL && !isSpinning && canSpin)
	{
		isSpinning = true;
		spin_start = GetTickCount64();
		DebugOut(L">>> Mario started spin attack! >>> \n");
	}
}

void CMario::HandleEnemyCollisionsInGodMode(LPCOLLISIONEVENT e)
{
	// Handle Goomba collisions
	if (CGoomba* goomba = dynamic_cast<CGoomba*>(e->obj))
	{
		if (goomba->GetState() != GOOMBA_STATE_DIE)
		{
			goomba->SetState(GOOMBA_STATE_DIE);
			return;
		}
	}
	// Handle ParaGoomba collisions
	else if (CParaGoomba* paraGoomba = dynamic_cast<CParaGoomba*>(e->obj))
	{
		if (paraGoomba->GetState() != PARAGOOMBA_STATE_DIE)
		{
			paraGoomba->SetState(PARAGOOMBA_STATE_DIE);
			return;
		}
	}
	// Handle Koopas collisions
	else if (Koopas* koopas = dynamic_cast<Koopas*>(e->obj))
	{
		if (koopas->GetState() != KOOPAS_STATE_SHELL)
		{
			koopas->SetState(KOOPAS_STATE_SHELL);
			return;
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL)
		{
			float shellDirection = (nx > 0) ? 1.0f : -1.0f;
			koopas->SetState(KOOPAS_STATE_SHELL_MOVING);
			koopas->SetSpeed(shellDirection * KOOPAS_SHELL_SPEED, 0);
			DebugOut(L">>> Mario kicked Koopa shell with spin attack! >>> \n");
			return;
		}
		else if (koopas->GetState() == KOOPAS_STATE_SHELL_MOVING)
		{
			// If shell is already moving, reverse its direction
			float vx, vy;
			koopas->GetSpeed(vx, vy);
			koopas->SetSpeed(-vx, vy);
			DebugOut(L">>> Mario reversed Koopa shell direction with spin attack! >>> \n");
			return;
		}
	}
	// Handle PiranhaPlant collisions
	else if (CPiranhaPlant* plant = dynamic_cast<CPiranhaPlant*>(e->obj))
	{
		plant->Delete();
		return;
	}
	// Handle Bullet collisions
	else if (CBullet* bullet = dynamic_cast<CBullet*>(e->obj))
	{
		bullet->Delete();
		return;
	}
}

void CMario::UpdatePowerMeter(DWORD dt)
{
	if (isRunning && isOnPlatform && !isSitting)
	{
		float speedFactor = min(abs(vx) / MARIO_RUNNING_SPEED, 1.0f);
		powerMeter += MARIO_PMETER_GAIN_RATE * speedFactor * dt;

		if (powerMeter > MARIO_PMETER_MAX)
			powerMeter = MARIO_PMETER_MAX;
	}
	else
	{
		powerMeter -= MARIO_PMETER_DECAY_RATE * dt;

		if (powerMeter < 0)
			powerMeter = 0;
	}
}

void CMario::UpdateHeldKoopas()
{
	if (!isHolding || heldKoopas == NULL)
		return;

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
		return;
	}

	UpdateHeldKoopasPosition();

	if (!CGame::GetInstance()->IsKeyDown(DIK_A))
	{
		ReleaseKoopas();
	}
}

void CMario::UpdateGlidingState()
{
	if (!isGliding)
	{
		isGliding = false;
		return;
	}
	if (isOnPlatform)
	{
		isGliding = false;
		return;
	}
	if (GetTickCount64() - glide_start > MARIO_GLIDE_TIMEOUT)
	{
		isGliding = false;
	}

	if (vy > 0)
		vy = MARIO_GLIDE_SPEED_Y;

}

void CMario::UpdateFlyingState()
{
	if (!isFlying)
		return;
	if (powerMeter <= 0)
	{
		isFlying = false;
		DebugOut(L">>> Mario flight ended due to depleted power meter! >>> \n");
		return;
	}

	powerMeter -= 0.01f;  
	if (powerMeter < 0) powerMeter = 0;

	vy = MARIO_FLY_SPEED_Y;

	if (isOnPlatform)
	{
		isFlying = false;
		DebugOut(L">>> Mario flight ended by landing! >>> \n");
	}
}


//
// Get animdation ID for big Mario
//
int CMario::GetAniIdBig()
{
	if (isKicking)
		return (nx >= 0) ? ID_ANI_MARIO_KICK_RIGHT : ID_ANI_MARIO_KICK_LEFT;

	// Handle holding animations
	if (isHolding)
	{
		int aniId = GetBigHoldingAniId();
		if (aniId != -1) return aniId;
	}

	// Handle jumping animations
	if (!isOnPlatform)
		return GetBigJumpingAniId();

	// Handle sitting
	if (isSitting)
		return (nx > 0) ? ID_ANI_MARIO_SIT_RIGHT : ID_ANI_MARIO_SIT_LEFT;

	// Handle idle
	if (vx == 0)
		return (nx > 0) ? ID_ANI_MARIO_IDLE_RIGHT : ID_ANI_MARIO_IDLE_LEFT;

	// Handle moving right
	if (vx > 0)
		return GetBigRightMovingAniId();

	// Handle moving left
	return GetBigLeftMovingAniId();
}

void CMario::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	int aniId = -1;

	if (state == MARIO_STATE_DIE)
		aniId = ID_ANI_MARIO_DIE;
	else if (level == MARIO_LEVEL_TAIL)
		aniId = GetAniIdTail();
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
		isRunning = true;
		maxVx = MARIO_RUNNING_SPEED;
		ax = MARIO_ACCEL_RUN_X;
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (isSitting) break;
		isRunning = true;
		maxVx = -MARIO_RUNNING_SPEED;
		ax = -MARIO_ACCEL_RUN_X;
		nx = -1;
		break;
	case MARIO_STATE_WALKING_RIGHT:
		if (isSitting) break;
		isRunning = false;
		maxVx = MARIO_WALKING_SPEED;
		ax = MARIO_ACCEL_WALK_X;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		if (isSitting) break;
		isRunning = false;
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
	case MARIO_STATE_FLY:
		isFlying = true;
		isGliding = false;
		fly_start = GetTickCount64();
		vy = MARIO_FLY_SPEED_Y;
		DebugOut(L">>> Mario started flying! >>> \n");
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
	case MARIO_STATE_SPIN_ATTACK:
		if (level == MARIO_LEVEL_TAIL) 
		{
			StartSpinAttack();
		}
		break;

	case MARIO_STATE_GLIDE:
		if (level == MARIO_LEVEL_TAIL && !isOnPlatform && vy > 0)
		{
			isGliding = true;
			glide_start = GetTickCount64();
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
	return (isHolding && heldKoopas && !heldKoopas->IsAboutToWakeUp()) ||
		(level == MARIO_LEVEL_TAIL && isSpinning);
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (level==MARIO_LEVEL_BIG || level==MARIO_LEVEL_TAIL)
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
			float width = (level == MARIO_LEVEL_TAIL && isSpinning) ?
				MARIO_TAIL_SPIN_BBOX_WIDTH : MARIO_BIG_BBOX_WIDTH;
			left = x - width / 2;
			top = y - MARIO_BIG_BBOX_HEIGHT / 2;
			right = left + width;
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

