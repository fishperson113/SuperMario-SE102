#include "MarioController.h"
#include"debug.h"
#include "Portal.h"
#include "Platform.h"
#include "KoopasController.h"

void MarioController::Update(float dt)
{
	if (!parentObject->IsActive()) return;

	auto velocity = parentObject->GetComponent<VelocityComponent>();
	auto transform = parentObject->GetComponent<TransformComponent>();
	auto collider = parentObject->GetComponent<ColliderComponent>();

	bool isLeftPressed = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;
	bool isRightPressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;
	bool isDownPressed = (GetAsyncKeyState(VK_DOWN) & 0x8000) != 0;
	bool isSPressed = (GetAsyncKeyState('S') & 0x8000) != 0;
	bool isAPressed = (GetAsyncKeyState('A') & 0x8000) != 0;

	// Get current velocity values
	VECTOR2 currentVelocity = velocity->GetVelocity();

	velocity->SetGrounded(false);
	// Reset horizontal velocity if no keys are pressed (simulate friction)
	if (isDownPressed) {

	}
	else if (isLeftPressed)	//Left
	{
		currentVelocity.x = 0.0f;
		if (isAPressed)	//+ A
			currentVelocity.x -= 1.5f;
		else
			currentVelocity.x -= 1.0f;
	}
	else if (isRightPressed)	//Right
	{
		currentVelocity.x = 0.0f;
		if (isAPressed)	//+ A
			currentVelocity.x += 1.5f;
		else
			currentVelocity.x += 1.0f;
	}
	else
		currentVelocity.x = 0.0f;

	if (currentVelocity.y <= -this->maxVy)
		this->canJump = false;

	if (!isSPressed) {  // Not Jumping
		if (!this->isOnGround) {
			currentVelocity.y += 0.2f; // Apply gravity when in the air (keep falling)
		}
		else {
			currentVelocity.y = 0.0f; // Reset vertical velocity when on the ground
		}
	}
	else if (isSPressed && !isDownPressed && this->canJump) {  // Jumping (Hold or Release)
		// Check if Mario can jump (only if not already at max jump speed)
		if (currentVelocity.y > -this->maxVy) { // If Mario's velocity is less than maxVy
			if (this->isOnGround) {  // Mario can jump again when on the ground
				currentVelocity.y = -1.0f; // Release Jump with normal speed
			}
			else {  // Mario is in the air, apply higher jump speed if holding jump
				currentVelocity.y = -2.0f; // Hold jump for higher velocity
			}
		}
	}

	velocity->SetVelocity(currentVelocity.x, currentVelocity.y);

	DebugOut(L"Mario: vx=%.2f\n", currentVelocity.x);
	DebugOut(L"Mario: vy=%.2f\n", currentVelocity.y);

	LPANIMATION ani = nullptr;
	auto animation = parentObject->GetComponent<AnimationComponent>();
	if (currentVelocity.x == 0) {	//Idle
		if (isDownPressed && currentVelocity.y == 0) {	//Sit + Not Jump
			if (lastDirection == -1)	//Left
				ani = CAnimations::GetInstance()->Get(ID_ANI_MARIO_SIT_LEFT);
			else
				ani = CAnimations::GetInstance()->Get(ID_ANI_MARIO_SIT_RIGHT);
		}
		else if (lastDirection == 1)	//Right
			ani = CAnimations::GetInstance()->Get(ID_ANI_MARIO_IDLE_RIGHT);
		else if (lastDirection == -1)	//Left
			ani = CAnimations::GetInstance()->Get(ID_ANI_MARIO_IDLE_LEFT);
		else
			ani = CAnimations::GetInstance()->Get(ID_ANI_MARIO_IDLE_RIGHT);
	}
	else {
		if (currentVelocity.y != 0) {	//Jump
			if (currentVelocity.x > 0) {	//Right
				lastDirection = 1;
				ani = isAPressed ? CAnimations::GetInstance()->Get(ID_ANI_MARIO_JUMP_RUN_RIGHT) : CAnimations::GetInstance()->Get(ID_ANI_MARIO_JUMP_WALK_RIGHT);
			}
			else if (currentVelocity.x < 0) {	//Left
				lastDirection = -1;
				ani = isAPressed ? CAnimations::GetInstance()->Get(ID_ANI_MARIO_JUMP_RUN_LEFT) : CAnimations::GetInstance()->Get(ID_ANI_MARIO_JUMP_WALK_LEFT);
			}
			else if (lastDirection == -1)
				ani = CAnimations::GetInstance()->Get(ID_ANI_MARIO_JUMP_WALK_LEFT);
			else
				ani = CAnimations::GetInstance()->Get(ID_ANI_MARIO_JUMP_WALK_RIGHT);
		}
		else {	//Not Jump
			if (currentVelocity.x > 0) {	//Right
				lastDirection = 1;
				ani = isAPressed ? CAnimations::GetInstance()->Get(ID_ANI_MARIO_RUNNING_RIGHT) : CAnimations::GetInstance()->Get(ID_ANI_MARIO_WALKING_RIGHT);
			}
			else if (currentVelocity.x < 0) {	//Left
				lastDirection = -1;
				ani = isAPressed ? CAnimations::GetInstance()->Get(ID_ANI_MARIO_RUNNING_LEFT) : CAnimations::GetInstance()->Get(ID_ANI_MARIO_WALKING_LEFT);
			}
		}
	}

	if (ani != nullptr && animation != nullptr) {
		animation->SetCurrentAnimation(ani);
	}

}

void MarioController::Awake()
{
	auto transform = parentObject->AddComponent<TransformComponent>();
	auto velocity = parentObject->AddComponent<VelocityComponent>();
	parentObject->AddComponent<AnimationComponent>();
	moveSpeed = 0.1f;
	velocity->SetSpeed(moveSpeed);

	auto collider = parentObject->AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 15, 22);
}

void MarioController::Start()
{
}
void MarioController::OnCollisionWith(LPCOLLISIONEVENT e)
{
	ScriptComponent::OnCollisionWith(e);

	auto velocity = parentObject->GetComponent<VelocityComponent>();
	if (!velocity) return;

	VECTOR2 currentVel = velocity->GetVelocity();
	if (e->ny < 0)
	{
		velocity->SetVelocity(currentVel.x, 0.0f);

		velocity->SetGrounded(true);
	}
	else if (e->ny > 0) {
		velocity->SetVelocity(currentVel.x, 0.0f);
	}
	else if (e->nx != 0)
	{
		velocity->SetVelocity(0.0f, currentVel.y);
	}

	if (e->obj->GetComponent<KoopasController>())
		OnCollisionWithKoopas(e);
	if ((CPlatform*)(e->obj))
		OnCollisionWithPlatform(e);
	if ((CPortal*)(e->obj))
		OnCollisionWithPortal(e);
}

void MarioController::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	DebugOut(L"Collision with Koopas detected: nx=%.1f, ny=%.1f\n", e->nx, e->ny);
}

void MarioController::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	DebugOut(L"Collision with Coin detected");
}

void MarioController::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	DebugOut(L"Collision with Portal detected");
}

void MarioController::SetCanJump(bool canJump)
{
	this->canJump = canJump;
}

void MarioController::OnCollisionWithPlatform(LPCOLLISIONEVENT e)
{
	DebugOut(L"Collision with Platform detected");
	this->SetCanJump(true);
}