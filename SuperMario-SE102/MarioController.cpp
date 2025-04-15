#include "MarioController.h"
#include"debug.h"
#include"KoopasController.h"

void MarioController::Update(float dt)
{
	if (!parentObject->IsActive()) return;

	auto velocity = parentObject->GetComponent<VelocityComponent>();
	auto transform = parentObject->GetComponent<TransformComponent>();
	auto collider = parentObject->GetComponent<ColliderComponent>();

	bool isLeftPressed = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;
	bool isRightPressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;

	// Get current velocity values
	VECTOR2 currentVelocity = velocity->GetVelocity();

	velocity->SetGrounded(false);
	// Reset horizontal velocity if no keys are pressed (simulate friction)
	if (!isLeftPressed && !isRightPressed) {
		currentVelocity.x = 0.0f;
	}
	else {
		// Apply movement based on input
		currentVelocity.x = 0.0f;
		if (isLeftPressed) currentVelocity.x -= 1.0f;
		if (isRightPressed) currentVelocity.x += 1.0f;
	}

	velocity->SetVelocity(currentVelocity.x, currentVelocity.y);

	LPANIMATION ani=nullptr;
	auto animation = parentObject->GetComponent<AnimationComponent>();
	if (currentVelocity.x > 0)
	{
		ani = CAnimations::GetInstance()->Get(rightAnimId);
	}
	else if (currentVelocity.x < 0)
	{
		ani = CAnimations::GetInstance()->Get(leftAnimId);
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(rightAnimId);
	}
	if (ani!=nullptr&&animation!=nullptr) {
		animation->SetCurrentAnimation(ani);
	}

}

void MarioController::Awake()
{
	auto transform = parentObject->AddComponent<TransformComponent>();
	auto velocity=parentObject->AddComponent<VelocityComponent>();
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
}

void MarioController::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	DebugOut(L"Collision with Koopas detected: nx=%.1f, ny=%.1f\n", e->nx, e->ny);
}

void MarioController::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
}

void MarioController::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
}
