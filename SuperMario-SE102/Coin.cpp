#include "Coin.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CCoin::CCoin()
{
	this->active = true;
	AddComponent<TransformComponent>();

	auto collider = AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 10, 15);
	collider->SetBlocking(false);
	collider->SetCollidableDirections(false, false, false, false);
	collider->SetBlocking(false);

	auto animation = AddComponent<AnimationComponent>();
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_COIN);
	animation->SetCurrentAnimation(ani);
	
}

CCoin::~CCoin()
{
}