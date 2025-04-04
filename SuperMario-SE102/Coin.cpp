#include "Coin.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CCoin::CCoin()
{
	AddComponent<TransformComponent>();
	AddComponent<AnimationComponent>();
}

CCoin::~CCoin()
{
}

void CCoin::Update(DWORD dt)
{
	if (this->active == false) return;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();

	auto transform = this->GetComponent<TransformComponent>();
	LPANIMATION ani = nullptr;
	auto animation = this->GetComponent<AnimationComponent>();
	ani = CAnimations::GetInstance()->Get(ID_ANI_COIN);
	if (ani != nullptr && animation != nullptr) {
		animation->SetCurrentAnimation(ani);
	}
}

void CCoin::Render()
{
	auto animation = GetComponent<AnimationComponent>();
	if (animation != nullptr) {
		auto transform = GetComponent<TransformComponent>();
		animation->Render();
	}
}

void CCoin::Start()
{
}

void CCoin::Awake()
{
	auto transform = this->GetComponent<TransformComponent>();
	transform->SetPosition(120.0f, 120.0f);

	auto animation = this->GetComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_COIN);
	animation->SetCurrentAnimation(ani);
}
