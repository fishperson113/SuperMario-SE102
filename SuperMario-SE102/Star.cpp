#include "Star.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CStar::CStar()
{
	AddComponent<TransformComponent>();
	AddComponent<AnimationComponent>();
}

CStar::~CStar()
{
}

void CStar::Update(DWORD dt)
{
	if (this->active == false) return;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();

	auto transform = this->GetComponent<TransformComponent>();
	LPANIMATION ani = nullptr;
	auto animation = this->GetComponent<AnimationComponent>();
	ani = CAnimations::GetInstance()->Get(ID_ANI_STAR);
	if (ani != nullptr && animation != nullptr) {
		animation->SetCurrentAnimation(ani);
	}
}

void CStar::Render()
{
	auto animation = GetComponent<AnimationComponent>();
	if (animation != nullptr) {
		auto transform = GetComponent<TransformComponent>();
		animation->Render();
	}
}

void CStar::Start()
{
}

void CStar::Awake()
{
	auto transform = this->GetComponent<TransformComponent>();
	transform->SetPosition(80.0f, 80.0f);

	auto animation = this->GetComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_STAR);
	animation->SetCurrentAnimation(ani);
}
