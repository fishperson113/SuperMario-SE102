#include "Turtle.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CTurtle::CTurtle()
{
	AddComponent<TransformComponent>();
	AddComponent<VelocityComponent>();
	AddComponent<AnimationComponent>();
}

CTurtle::~CTurtle()
{
}

void CTurtle::Update(DWORD dt)
{
	if (this->active == false) return;
	int BackBufferWidth = CGame::GetInstance()->GetBackBufferWidth();

	auto velocity = this->GetComponent<VelocityComponent>();
	auto transform = this->GetComponent<TransformComponent>();

	LPANIMATION ani = nullptr;
	auto animation = this->GetComponent<AnimationComponent>();

	if (velocity->GetVelocity().x > 0)
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_TURTLE + 1);
	}
	else if (velocity->GetVelocity().x < 0)
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_TURTLE - 1);
	}
	else
	{
		ani = CAnimations::GetInstance()->Get(ID_ANI_TURTLE + 1);
	}
	if (ani != nullptr && animation != nullptr) {
		animation->SetCurrentAnimation(ani);
	}

	Move();
}

void CTurtle::Render()
{
	auto animation = GetComponent<AnimationComponent>();
	if (animation != nullptr) {
		auto transform = GetComponent<TransformComponent>();
		animation->Render();
	}
}

void CTurtle::Start()
{
}

void CTurtle::Awake()
{
	auto transform = this->GetComponent<TransformComponent>();
	transform->SetPosition(60.0f, 60.0f);

	auto velocity = this->GetComponent<VelocityComponent>();
	auto animation = this->GetComponent<AnimationComponent>();

	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_TURTLE + 1);
	animation->SetCurrentAnimation(ani);
}

void CTurtle::Move()
{
	auto velocity = GetComponent<VelocityComponent>();
	velocity->SetVelocity(0.1f, 0.0f);

	auto transform = GetComponent<TransformComponent>();
	int posX = transform->GetPositionX();
	int posY = transform->GetPositionY();
	transform->SetPosition(posX + 1, posY);
}
