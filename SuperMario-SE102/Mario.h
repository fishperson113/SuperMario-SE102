#pragma once
#include "GameObject.h"

#include "Animation.h"
#include "Animations.h"
#include "Pipe.h"
#include "debug.h"
#include "KoopaParatroopa.h"


#define MARIO_WALKING_SPEED		0.07f
#define MARIO_RUNNING_SPEED		0.16f

#define MARIO_ACCEL_WALK_X	0.0005f
#define MARIO_ACCEL_RUN_X	0.0004f
#define MARIO_PMETER_MAX          6        
#define MARIO_PMETER_GAIN_RATE    0.016f  
#define MARIO_PMETER_DECAY_RATE   0.008f

#define MARIO_JUMP_SPEED_Y		0.5f
#define MARIO_JUMP_RUN_SPEED_Y	0.6f

#define MARIO_GRAVITY			0.002f

#define MARIO_JUMP_DEFLECT_SPEED  0.4f

#define MARIO_STATE_FLY 810
#define MARIO_FLY_SPEED_Y -0.2f 

#define MARIO_STATE_DIE				-10
#define MARIO_STATE_IDLE			0
#define MARIO_STATE_WALKING_RIGHT	100
#define MARIO_STATE_WALKING_LEFT	200

#define MARIO_STATE_JUMP			300
#define MARIO_STATE_RELEASE_JUMP    301

#define MARIO_STATE_RUNNING_RIGHT	400
#define MARIO_STATE_RUNNING_LEFT	500

#define MARIO_STATE_SIT				600
#define MARIO_STATE_SIT_RELEASE		601

#define MARIO_KICK_TIMEOUT 150

#define MARIO_STATE_SPIN_ATTACK           700
#define MARIO_SPIN_ATTACK_TIMEOUT         400
#define MARIO_SPIN_ATTACK_COOLDOWN        400

#define MARIO_STATE_GLIDE 800
#define MARIO_GLIDE_SPEED_Y 0.05f  // Slower falling speed while gliding
#define MARIO_GLIDE_TIMEOUT 5000   // Max glide time in milliseconds

#define MARIO_PIPE_ENTRY_TIME       800    // Time to enter pipe animation
#define MARIO_PIPE_EXIT_TIME        800    // Time to exit pipe animation
#define MARIO_PIPE_MOVE_SPEED       0.2f 

#pragma region ANIMATION_ID
//BIG MARIO
#define ID_ANI_MARIO_IDLE_RIGHT 400
#define ID_ANI_MARIO_IDLE_LEFT 401

#define ID_ANI_MARIO_WALKING_RIGHT 500
#define ID_ANI_MARIO_WALKING_LEFT 501

#define ID_ANI_MARIO_RUNNING_RIGHT 600
#define ID_ANI_MARIO_RUNNING_LEFT 601

#define ID_ANI_MARIO_JUMP_WALK_RIGHT 700
#define ID_ANI_MARIO_JUMP_WALK_LEFT 701

#define ID_ANI_MARIO_JUMP_RUN_RIGHT 800
#define ID_ANI_MARIO_JUMP_RUN_LEFT 801

#define ID_ANI_MARIO_SIT_RIGHT 900
#define ID_ANI_MARIO_SIT_LEFT 901

#define ID_ANI_MARIO_BRACE_RIGHT 1000
#define ID_ANI_MARIO_BRACE_LEFT 1001

#define ID_ANI_MARIO_DIE 999

// BIG MARIO HOLDING SHELL
#define ID_ANI_MARIO_HOLD_IDLE_RIGHT 2001
#define ID_ANI_MARIO_HOLD_IDLE_LEFT 2002
#define ID_ANI_MARIO_HOLD_RUNNING_RIGHT 2003
#define ID_ANI_MARIO_HOLD_RUNNING_LEFT 2004
#define ID_ANI_MARIO_KICK_RIGHT 2005
#define ID_ANI_MARIO_KICK_LEFT 2006

// SMALL MARIO
#define ID_ANI_MARIO_SMALL_IDLE_RIGHT 1100
#define ID_ANI_MARIO_SMALL_IDLE_LEFT 1102

#define ID_ANI_MARIO_SMALL_WALKING_RIGHT 1200
#define ID_ANI_MARIO_SMALL_WALKING_LEFT 1201

#define ID_ANI_MARIO_SMALL_RUNNING_RIGHT 1300
#define ID_ANI_MARIO_SMALL_RUNNING_LEFT 1301

#define ID_ANI_MARIO_SMALL_BRACE_RIGHT 1400
#define ID_ANI_MARIO_SMALL_BRACE_LEFT 1401

#define ID_ANI_MARIO_SMALL_JUMP_WALK_RIGHT 1500
#define ID_ANI_MARIO_SMALL_JUMP_WALK_LEFT 1501

#define ID_ANI_MARIO_SMALL_JUMP_RUN_RIGHT 1600
#define ID_ANI_MARIO_SMALL_JUMP_RUN_LEFT 1601

// SMALL MARIO HOLDING SHELL 
#define ID_ANI_MARIO_SMALL_HOLD_IDLE_RIGHT 2100
#define ID_ANI_MARIO_SMALL_HOLD_IDLE_LEFT 2101
#define ID_ANI_MARIO_SMALL_HOLD_RUNNING_RIGHT 2102
#define ID_ANI_MARIO_SMALL_HOLD_RUNNING_LEFT 2103
#define ID_ANI_MARIO_SMALL_KICK_RIGHT 2104
#define ID_ANI_MARIO_SMALL_KICK_LEFT 2105

// BIG MARIO WITH TAIL (RACCOON MARIO)
#define ID_ANI_MARIO_TAIL_IDLE_RIGHT 3000
#define ID_ANI_MARIO_TAIL_IDLE_LEFT 3001

#define ID_ANI_MARIO_TAIL_WALKING_RIGHT 3100
#define ID_ANI_MARIO_TAIL_WALKING_LEFT 3101

#define ID_ANI_MARIO_TAIL_RUNNING_RIGHT 3200
#define ID_ANI_MARIO_TAIL_RUNNING_LEFT 3201

#define ID_ANI_MARIO_TAIL_JUMP_WALK_RIGHT 3300
#define ID_ANI_MARIO_TAIL_JUMP_WALK_LEFT 3301

#define ID_ANI_MARIO_TAIL_JUMP_RUN_RIGHT 3400
#define ID_ANI_MARIO_TAIL_JUMP_RUN_LEFT 3401

#define ID_ANI_MARIO_TAIL_SIT_RIGHT 3500
#define ID_ANI_MARIO_TAIL_SIT_LEFT 3501

#define ID_ANI_MARIO_TAIL_BRACE_RIGHT 3600
#define ID_ANI_MARIO_TAIL_BRACE_LEFT 3601

// BIG MARIO WITH TAIL HOLDING SHELL
#define ID_ANI_MARIO_TAIL_HOLD_IDLE_RIGHT 3700
#define ID_ANI_MARIO_TAIL_HOLD_IDLE_LEFT 3701
#define ID_ANI_MARIO_TAIL_HOLD_RUNNING_RIGHT 3800
#define ID_ANI_MARIO_TAIL_HOLD_RUNNING_LEFT 3801
#define ID_ANI_MARIO_TAIL_KICK_RIGHT 3900
#define ID_ANI_MARIO_TAIL_KICK_LEFT 3901

#define ID_ANI_MARIO_TAIL_SPIN_ATTACK     4010 

// RACCOON MARIO GLIDE ANIMATIONS
#define ID_ANI_MARIO_TAIL_GLIDE_RIGHT 4100
#define ID_ANI_MARIO_TAIL_GLIDE_LEFT 4101

#define ID_ANI_MARIO_TAIL_FLY_RIGHT 4200
#define ID_ANI_MARIO_TAIL_FLY_LEFT 4201

#pragma endregion

#define GROUND_Y 160.0f

#define	MARIO_LEVEL_SMALL	1
#define	MARIO_LEVEL_BIG		2
#define MARIO_LEVEL_TAIL     3

#define MARIO_BIG_BBOX_WIDTH  14
#define MARIO_TAIL_SPIN_BBOX_WIDTH  22
#define MARIO_BIG_BBOX_HEIGHT 24
#define MARIO_BIG_SITTING_BBOX_WIDTH  14
#define MARIO_BIG_SITTING_BBOX_HEIGHT 16

#define MARIO_SIT_HEIGHT_ADJUST ((MARIO_BIG_BBOX_HEIGHT-MARIO_BIG_SITTING_BBOX_HEIGHT)/2)

#define MARIO_SMALL_BBOX_WIDTH  13
#define MARIO_SMALL_BBOX_HEIGHT 12


#define MARIO_UNTOUCHABLE_TIME 2500

#define MARIO_STATE_PIPE 920
class Koopas;
class CPlatform;
class CMario : public CGameObject
{
	Koopas* heldKoopas;  
	BOOLEAN isHolding;
	BOOLEAN isSitting;
	float maxVx;
	float ax;				// acceleration on x 
	float ay;				// acceleration on y 
	float powerMeter;
	CPlatform* platform; // The platform that Mario is on
	int level; 
	int untouchable; 
	ULONGLONG untouchable_start;
	BOOLEAN isOnPlatform;
	int coin; 
	int points;
	BOOLEAN isKicking;
	ULONGLONG kick_start;
	ULONGLONG teleport_exit_start;
	BOOLEAN isSpinning;
	ULONGLONG spin_start;
	ULONGLONG spin_cooldown_start;
	bool canSpin;

	BOOLEAN isGliding;
	ULONGLONG glide_start;

	BOOLEAN isFlying;
	ULONGLONG fly_start;
	BOOLEAN isRunning;
	float pipe_offset;  // Offset for pipe entry/exit animation
	BOOLEAN isTeleporting;
	ULONGLONG teleport_start;
	float teleport_target_x, teleport_target_y;
	PipeDirection teleport_exit_direction;
	PipeDirection current_pipe_direction;

	bool isUnderground = false;

	void UpdateHeldKoopasPosition();
	void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithCoinBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroom(LPCOLLISIONEVENT e);
	void OnCollisionWithMushroomBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithFallPitch(LPCOLLISIONEVENT e);
	void OnCollisionWithParaGoomba(LPCOLLISIONEVENT e);
	void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperLeaf(LPCOLLISIONEVENT e);
	void OnCollisionWithSuperLeafBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithCheckpoint(LPCOLLISIONEVENT e);
	void OnCollisionWithBullet(LPCOLLISIONEVENT e);
	void OnCollisionWithPipe(LPCOLLISIONEVENT e);
	void OnCollisionWithKoopaParatroopa(LPCOLLISIONEVENT e);
	void OnCollisionWithBoomerang(LPCOLLISIONEVENT e); 
	void OnCollisionWithBoomerangBro(LPCOLLISIONEVENT e);

	int GetAniIdBig();
	int GetAniIdSmall();
	int GetAniIdTail();

	int GetSmallHoldingAniId();
	int GetSmallJumpingAniId();
	int GetSmallRightMovingAniId();
	int GetSmallLeftMovingAniId();

	int GetBigHoldingAniId();
	int GetBigJumpingAniId();
	int GetBigRightMovingAniId();
	int GetBigLeftMovingAniId();

	int GetTailHoldingAniId();
	int GetTailJumpingAniId();
	int GetTailRightMovingAniId();
	int GetTailLeftMovingAniId();


	void HandleKoopasJumpedOn(Koopas* koopas);
	void HandleKoopasSideCollision(Koopas* koopas, LPCOLLISIONEVENT e);
	void KickKoopasShell(Koopas* koopas, LPCOLLISIONEVENT e);
	void TakeDamage();
	void LevelDown();
	void StartSpinAttack();
	void HandleEnemyCollisionsInGodMode(LPCOLLISIONEVENT e);

	void UpdateVelocity(DWORD dt);
	void UpdateUntouchableState();
	void UpdateKickingState();
	void UpdateSpinningState();
	void UpdatePowerMeter(DWORD dt);
	void UpdateHeldKoopas();
	void UpdateGlidingState();
	void UpdateFlyingState();
	void UpdateTeleportingState();

	void AlignWithPipe(CPipe* pipe, PipeDirection direction);
	void StartPipeMovement(PipeDirection direction, bool isEntry);
	void UpdatePipeMovement();
	bool CanEnterPipe(CPipe* pipe, LPCOLLISIONEVENT e);

public:
	CMario(float x, float y) : CGameObject(x, y)
	{
		isSitting = false;
		maxVx = 0.0f;
		ax = 0.0f;
		ay = MARIO_GRAVITY; 
		level = MARIO_LEVEL_SMALL;
		untouchable = 0;
		untouchable_start = -1;
		isOnPlatform = false;
		coin = 10;
		heldKoopas = NULL;
		isHolding = false;
		isKicking = false;
		kick_start = 0;
		powerMeter = 0.0f;
		isSpinning = false; 
		spin_start = 0;
		spin_cooldown_start = 0;
		canSpin = true;
		isGliding = false;
		glide_start = 0;
		platform = NULL;
		isFlying = false;
		fly_start = 0;
		isRunning = false;
		isTeleporting = false;
		teleport_start = 0;
		teleport_target_x = 0;
		teleport_target_y = 0;
		teleport_exit_direction = PipeDirection::UP;
		pipe_offset = 0.0f;
		points = 0;
		teleport_exit_start = 0;
		current_pipe_direction = PipeDirection::UP;
	}

	void HoldKoopas(Koopas* koopas);
	void ReleaseKoopas();
	bool IsHolding() { return isHolding; }
	bool IsSpinning() { return isSpinning; }
	bool IsGliding() { return isGliding; }
	bool IsFlying() { return isFlying; }
	void EndFly() { isFlying = false; }
	void SetIsOnPlatform(bool isOn) { isOnPlatform = isOn; }
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void SetState(int state);
	bool IsGodMode();
	int IsCollidable()
	{ 
		return (state != MARIO_STATE_DIE); 
	}
	bool IsTeleporting() { return isTeleporting; }
	bool IsOnPlatform() { return isOnPlatform; }

	int IsBlocking() { return (state != MARIO_STATE_DIE && untouchable==0); }
	int IsDynamic() { return 1; } // Mario is always dynamic
	void OnNoCollision(DWORD dt);
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void OnOverlapWith(LPGAMEOBJECT e) {};
	void OnCollisionExit(LPGAMEOBJECT obj);

	float GetPowerMeter() { return powerMeter; }
	void SetLevel(int l);
	int GetLevel() { return level; }
	int GetUntouchable() { return untouchable; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount64(); }
	void EndGlide() { isGliding = false; }
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	int GetCoin() { return coin; }
	int GetPoints() { return points; }

	void SetIsUnderground(bool underground) { isUnderground = underground; }
	bool GetIsUnderground() { return isUnderground; }
};