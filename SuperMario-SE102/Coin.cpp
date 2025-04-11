#include "Coin.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CCoin::CCoin()
{
	this->active = true;
	AddComponent<TransformComponent>();
	auto animation = AddComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_COIN);
	animation->SetCurrentAnimation(ani);
}

CCoin::~CCoin()
{
}