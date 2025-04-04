#include "Goomba.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CGoomba::CGoomba()
{
	AddComponent<TransformComponent>();
	AddComponent<VelocityComponent>();
	AddComponent<AnimationComponent>();
}

CGoomba::~CGoomba()
{
}

void CGoomba::Update(DWORD dt)
{
	if (this->active == false) return;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();

	auto velocity = this->GetComponent<VelocityComponent>();
	auto transform = this->GetComponent<TransformComponent>();

	LPANIMATION ani = nullptr;
	auto animation = this->GetComponent<AnimationComponent>();

	if (velocity->GetVelocity().x > 0)
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_GOOMBA_WALKING);
	}
	else if (velocity->GetVelocity().x < 0)
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_GOOMBA_WALKING);
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_GOOMBA_WALKING);
	}
	if (ani != nullptr && animation != nullptr) {
		animation->SetCurrentAnimation(ani);
	}

	Move();
}

void CGoomba::Render()
{
	auto animation = GetComponent<AnimationComponent>();
	if (animation != nullptr) {
		auto transform = GetComponent<TransformComponent>();
		animation->Render();
	}
}

void CGoomba::Start()
{
}

void CGoomba::Awake()
{
	auto transform = this->GetComponent<TransformComponent>();
	transform->SetPosition(150.0f, 150.0f);

	auto velocity = this->GetComponent<VelocityComponent>();
	auto animation = this->GetComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_GOOMBA_WALKING);
	animation->SetCurrentAnimation(ani);
}

void CGoomba::Move()
{
	auto velocity = GetComponent<VelocityComponent>();
	velocity->SetVelocity(0.1f, 0.0f);

	auto transform = GetComponent<TransformComponent>();
	int posX = transform->GetPositionX();
	int posY = transform->GetPositionY();
	transform->SetPosition(posX + 1, posY);
}
