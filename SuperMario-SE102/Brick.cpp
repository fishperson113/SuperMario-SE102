#include "Brick.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CBrick::CBrick()
{
	this->active = true;
	AddComponent<TransformComponent>();
	auto animation=AddComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_BRICK);
	animation->SetCurrentAnimation(ani);
}

CBrick::~CBrick()
{
}

