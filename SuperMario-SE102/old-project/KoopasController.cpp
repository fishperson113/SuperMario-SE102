#include "KoopasController.h"
#include <typeinfo>
#include "Coin.h"

void KoopasController::Update(float dt)
{

}

void KoopasController::Awake()
{
	parentObject->AddComponent<TransformComponent>();
	auto velocity=parentObject->AddComponent<VelocityComponent>();
	auto animation = parentObject->AddComponent<AnimationComponent>();

	auto collider = parentObject->AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 15, 15);
	LPANIMATION ani = CAnimations::GetInstance()->Get(5000);
	animation->SetCurrentAnimation(ani);

	velocity->SetVelocity(-1.0f, 0.0f);
}

void KoopasController::Start()
{
}

void KoopasController::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->GetComponent<ColliderComponent>()->IsBlocking()) return;

	ScriptComponent::OnCollisionWith(e);
    auto velocity = parentObject->GetComponent<VelocityComponent>();
    if (!velocity) return;

    if (e->nx != 0) {

        float currentVx = velocity->GetVelocity().x;

        velocity->SetVelocity(-currentVx, velocity->GetVelocity().y);

        DebugOut(L"Koopas changed direction: vx=%.2f\n", -currentVx);
    }

    if (e->ny < 0) {
        velocity->SetGrounded(true);
    }
}
