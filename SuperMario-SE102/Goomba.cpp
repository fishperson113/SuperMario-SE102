#include "Goomba.h"
#include "AssetIDs.h"

CGoomba::CGoomba()
{
	AddComponent<GoombaController>();
}

CGoomba::~CGoomba()
{
}

void CGoomba::Awake()
{
	auto goombaController = this->GetComponent<GoombaController>();
	auto transform = this->GetComponent<GoombaController>()->GetParentObject()->GetComponent<TransformComponent>();
	auto animation = this->GetComponent<GoombaController>()->GetParentObject()->GetComponent<AnimationComponent>();
	auto velocity = this->GetComponent<GoombaController>()->GetParentObject()->GetComponent<VelocityComponent>();

	goombaController->Awake();
}

void CGoomba::Update(DWORD dt)
{
	if (this->active == false) return;

	CGameObject::Update(dt);
	if (goombaController) {
		goombaController->Update(dt);
	}
}

void CGoomba::Render()
{
	CGameObject::Render();
}


