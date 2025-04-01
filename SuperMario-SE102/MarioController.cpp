#include "MarioController.h"

void MarioController::Update(float dt)
{
	if (!parentObject->IsActive()) return;
	auto velocity = parentObject->GetComponent<VelocityComponent>();

	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();
	auto transform = parentObject->GetComponent<TransformComponent>();

	bool isLeftPressed = (GetAsyncKeyState(VK_LEFT) & 0x8000) != 0;
	bool isRightPressed = (GetAsyncKeyState(VK_RIGHT) & 0x8000) != 0;

	VECTOR2 direction(0,velocity->GetVelocity().y);
	if (isLeftPressed) direction.x -= 1.0f;
	if (isRightPressed) direction.x += 1.0f;

	velocity->SetVelocity(direction.x, direction.y);


	LPANIMATION ani=nullptr;
	auto animation = parentObject->GetComponent<AnimationComponent>();
	if (direction.x > 0)
	{
		ani = CAnimations::GetInstance()->Get(rightAnimId);
	}
	else if (direction.x < 0)
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
	transform->SetPosition(10.0f, 130.0f);
	auto velocity=parentObject->AddComponent<VelocityComponent>();
	parentObject->AddComponent<AnimationComponent>();
	moveSpeed = 100.0f;

	velocity->SetSpeed(moveSpeed);
	velocity->SetVelocity(0.0f, 0.0f);
}

void MarioController::Start()
{
}
