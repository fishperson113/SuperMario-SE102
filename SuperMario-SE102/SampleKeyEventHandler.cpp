#include "SampleKeyEventHandler.h"

#include "debug.h"
#include "Game.h"

#include "Mario.h"
#include "PlayScene.h"

void CSampleKeyHandler::OnKeyDown(int KeyCode)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();
	CMario* mario = (CMario *)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!mario) return;

	// Handle pause key (P)
	if (KeyCode == DIK_P)
	{
		scene->TogglePause();
		DebugOut(L"[INFO] Game %s\n", scene->IsPaused() ? L"Paused" : L"Resumed");
		return;
	}
	// If the game is paused, don't process other keys except for P
	if (scene->IsPaused())
		return;

	switch (KeyCode)
	{
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT);
		break;
	case DIK_S:
		if (mario->GetLevel() == MARIO_LEVEL_TAIL &&
			mario->IsOnPlatform() &&
			mario->GetPowerMeter() >= MARIO_PMETER_MAX)
		{
			mario->SetState(MARIO_STATE_FLY);
		}
		else
		{
			// Normal jump if can't fly
			mario->SetState(MARIO_STATE_JUMP);
		}
		break;
	case DIK_1:
		mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_2:
		mario->SetLevel(MARIO_LEVEL_BIG);
		break;
	case DIK_3:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_0:
		mario->SetState(MARIO_STATE_DIE);
		break;
	case DIK_A:
	if (mario->GetLevel() == MARIO_LEVEL_TAIL)
		mario->SetState(MARIO_STATE_SPIN_ATTACK);
		break;
	case DIK_R: // reset
		scene->Reload();
		break;
	}
}

void CSampleKeyHandler::OnKeyUp(int KeyCode)
{
	//DebugOut(L"[INFO] KeyUp: %d\n", KeyCode);
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!mario) return;
	if (scene->IsPaused())
		return;
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetState() == MARIO_STATE_PIPE || mario->IsTeleporting())
			return;
		mario->SetState(MARIO_STATE_RELEASE_JUMP);
		if (mario->IsFlying())
			mario->EndFly();
		if (mario->IsGliding())
			mario->EndGlide();
		break;
	case DIK_DOWN:
		mario->SetState(MARIO_STATE_SIT_RELEASE);
		break;
	case DIK_A:
		if (mario->IsHolding())
			mario->ReleaseKoopas();
		break;
	}
}

void CSampleKeyHandler::KeyState(BYTE *states)
{
	LPPLAYSCENE scene = (LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene();

	LPGAME game = CGame::GetInstance();
	CMario* mario = (CMario*)((LPPLAYSCENE)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (!mario) return;
	if (scene->IsPaused())
		return;
	if (game->IsKeyDown(DIK_S) && mario->GetLevel() == MARIO_LEVEL_TAIL)
	{
		float vx,vy;
		mario->GetSpeed(vx, vy);

		if (!mario->IsOnPlatform() && vy > 0 && !mario->IsFlying())
		{
			mario->SetState(MARIO_STATE_GLIDE);
		}
	}
	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
		else
			mario->SetState(MARIO_STATE_WALKING_RIGHT);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_A))
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
		else
			mario->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
		mario->SetState(MARIO_STATE_IDLE);
}