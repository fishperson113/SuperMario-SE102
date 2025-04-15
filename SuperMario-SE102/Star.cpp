#include "Star.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CStar::CStar()
{
	this->active = true;
	AddComponent<TransformComponent>();

	auto collider = AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 16, 16);
	collider->SetCollidableDirections(false, false, false, false);
	collider->SetBlocking(false);

	auto animation = AddComponent<AnimationComponent>();
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_STAR);
	animation->SetCurrentAnimation(ani);
}

CStar::~CStar()
{
}
