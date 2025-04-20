#include "JumpingGoombaController.h"

void CJumpingGoombaController::Update(float dt)
{
	auto velocity = parentObject->GetComponent<VelocityComponent>();
	auto currentVelocity = velocity->GetVelocity();
	if (currentVelocity.y == 0.0f)
		currentVelocity.y = -2.0f;
	else if (currentVelocity.y != 0.0f)
		currentVelocity.y += 0.05f;
	velocity->SetVelocity(currentVelocity.x, currentVelocity.y);
}

void CJumpingGoombaController::Awake()
{
	parentObject->AddComponent<TransformComponent>();

	auto velocity = parentObject->AddComponent<VelocityComponent>();
	velocity->SetVelocity(-1.0f, 0.0f);

	auto animation = parentObject->AddComponent<AnimationComponent>();

	auto collider = parentObject->AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 19, 18);
	LPANIMATION ani = CAnimations::GetInstance()->Get(5002);
	animation->SetCurrentAnimation(ani);
}

void CJumpingGoombaController::Start()
{
}

void CJumpingGoombaController::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->GetComponent<ColliderComponent>()->IsBlocking()) return;

	ScriptComponent::OnCollisionWith(e);
	auto velocity = parentObject->GetComponent<VelocityComponent>();
	if (!velocity) return;

	if (e->nx != 0) {

		float currentVx = velocity->GetVelocity().x;

		velocity->SetVelocity(-currentVx, velocity->GetVelocity().y);

		DebugOut(L"Goomba changed direction: vx=%.2f\n", -currentVx);
	}

	if (e->ny < 0) {
		velocity->SetGrounded(true);
	}
}
