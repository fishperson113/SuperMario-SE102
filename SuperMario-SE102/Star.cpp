#include "Star.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CStar::CStar()
{
	this->active = true;
	AddComponent<TransformComponent>();
	auto animation = AddComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_STAR);
	animation->SetCurrentAnimation(ani);
}

CStar::~CStar()
{
}
