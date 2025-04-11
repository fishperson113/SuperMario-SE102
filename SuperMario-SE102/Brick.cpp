#include "Brick.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CBrick::CBrick()
{
	this->active = true;
	AddComponent<TransformComponent>();
	auto animation=AddComponent<AnimationComponent>();
	auto collider = AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 16, 16);
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_BRICK);
	animation->SetCurrentAnimation(ani);
}

CBrick::~CBrick()
{
}

