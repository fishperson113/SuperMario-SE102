#include "MarioController.h"
#include"debug.h"
extern vector<LPGAMEOBJECT>* coObjects;

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

	if (collider && velocity && coObjects)
	{
		velocity->SetEnabled(false);
	
		CCollision::GetInstance()->Process(this->GetParentObject(), dt, coObjects);

		velocity->SetEnabled(true);
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

	auto collider = parentObject->AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 25, 25);
}

void MarioController::Start()
{
}
void MarioController::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->GetComponent<ColliderComponent>()->IsBlocking()) return;

	auto velocity = parentObject->GetComponent<VelocityComponent>();
	if (!velocity) return;

	if (e->ny != 0) 
	{
		VECTOR2 currentVel = velocity->GetVelocity();
		velocity->SetVelocity(currentVel.x, 0.0f);

		if (e->ny < 0) {
			DebugOut(L"Mario is on ground\n");
		}
	}
	else if (e->nx != 0) 
	{
		VECTOR2 currentVel = velocity->GetVelocity();
		velocity->SetVelocity(0.0f, currentVel.y);

		DebugOut(L"Horizontal collision detected: nx=%.1f\n", e->nx);
	}
}