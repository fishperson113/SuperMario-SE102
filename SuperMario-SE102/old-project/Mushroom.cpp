#include "Mushroom.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CMushroom::CMushroom()
{
	AddComponent<TransformComponent>();
	AddComponent<VelocityComponent>();
	AddComponent<AnimationComponent>();
}

CMushroom::~CMushroom()
{
}

void CMushroom::Update(DWORD dt)
{
	if (this->active == false) return;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();

	auto velocity = this->GetComponent<VelocityComponent>();
	auto transform = this->GetComponent<TransformComponent>();
	
	LPANIMATION ani = nullptr;
	auto animation = this->GetComponent<AnimationComponent>();

	if (velocity->GetVelocity().x > 0)
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM);
	}
	else if (velocity->GetVelocity().x < 0)
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM);
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM);
	}
	if (ani != nullptr && animation != nullptr) {
		animation->SetCurrentAnimation(ani);
	}

	Move();
}

void CMushroom::Render()
{
	auto animation = GetComponent<AnimationComponent>();
	if (animation != nullptr) {
		auto transform = GetComponent<TransformComponent>();
		animation->Render();
	}
}

void CMushroom::Start()
{
}

void CMushroom::Awake()
{
	auto transform = this->GetComponent<TransformComponent>();
	transform->SetPosition(100.0f, 100.0f);

	auto velocity = this->GetComponent<VelocityComponent>();
	auto animation = this->GetComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_MUSHROOM);
	animation->SetCurrentAnimation(ani);
}

void CMushroom::Move()
{
	auto velocity = GetComponent<VelocityComponent>();
	velocity->SetVelocity(0.1f, 0.0f);

	auto transform = GetComponent<TransformComponent>();
	int posX = transform->GetPositionX();
	int posY = transform->GetPositionY();
	transform->SetPosition(posX + 1, posY);
}
