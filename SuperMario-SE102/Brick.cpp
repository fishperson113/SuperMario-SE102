#include "Brick.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CBrick::CBrick()
{
	AddComponent<TransformComponent>();
	AddComponent<AnimationComponent>();
}

CBrick::~CBrick()
{
}

void CBrick::Update(DWORD dt)
{
	if (this->active == false) return;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();

	auto transform = this->GetComponent<TransformComponent>();
	LPANIMATION ani = nullptr;
	auto animation = this->GetComponent<AnimationComponent>();
	ani = CAnimations::GetInstance()->Get(ID_ANI_BRICK);
	if (ani != nullptr && animation != nullptr) {
		animation->SetCurrentAnimation(ani);
	}
}

void CBrick::Render()
{
	auto animation = GetComponent<AnimationComponent>();
	if (animation != nullptr) {
		auto transform = GetComponent<TransformComponent>();
		animation->Render();
	}
}

void CBrick::Start()
{
}

void CBrick::Awake()
{
	auto transform = this->GetComponent<TransformComponent>();
	transform->SetPosition(100.0f, 100.0f);

	auto animation = this->GetComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_BRICK);
	animation->SetCurrentAnimation(ani);
}
