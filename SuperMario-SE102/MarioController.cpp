#include "MarioController.h"
#include"debug.h"
#include "Coin.h"
#include <typeinfo>
#include "Goomba.h"
#include "Koopas.h"
#include "Coin.h"
#include "Portal.h"

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
		if (isLeftPressed) currentVelocity.x -= 100.0f;
		if (isRightPressed) currentVelocity.x += 100.0f;
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
	transform->SetPosition(10.0f, 130.0f);

	auto velocity=parentObject->AddComponent<VelocityComponent>();
	parentObject->AddComponent<AnimationComponent>();
	moveSpeed = 100.0f;
	velocity->SetSpeed(moveSpeed);
	velocity->SetVelocity(0.0f, 0.0f);

	//auto collider = parentObject->AddComponent<ColliderComponent>();
	//collider->SetBoundingBox(0, 0, 25, 25);
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

	if (typeid(*e->obj) == typeid(CCoin)) {
		DebugOut(L"Collision with Coin detected\n");
		OnCollisionWithCoin(e);
	}
	else if (typeid(*e->obj) == typeid(CGoomba)) {
		DebugOut(L"Collision with Goomba detected\n");
		OnCollisionWithGoomba(e);
	}
	else if (typeid(*e->obj) == typeid(Koopas)) {
		DebugOut(L"Collision with Koopas detected\n");
		OnCollisionWithKoopas(e);
	}
	else if (typeid(*e->obj) == typeid(CPortal))
	{
		DebugOut(L"Collision with Portal detected\n");
		OnCollisionWithPortal(e);
	}
	else
	{
		DebugOut(L"Collision with unknown object detected\n");
	}
}

void MarioController::OnCollisionWithCoin(LPCOLLISIONEVENT e)
{
	CCoin* coin = static_cast<CCoin*>(e->obj);
	coin->Deactivate();
	//coin++;
}

void MarioController::OnCollisionWithGoomba(LPCOLLISIONEVENT e)
{
	CGoomba* goomba = static_cast<CGoomba*>(e->obj);
	goomba->Deactivate();
}

void MarioController::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	Koopas* koopas = static_cast<Koopas*>(e->obj);
	koopas->Deactivate();
}

void MarioController::OnCollisionWithPortal(LPCOLLISIONEVENT e)
{
	CPortal* portal = static_cast<CPortal*>(e->obj);
	CGame::GetInstance()->InitiateSwitchScene(portal->GetSceneId());
}
