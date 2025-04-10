#include "Mario.h"
#include "AssetIDs.h"

CMario::CMario()
{
	AddComponent<MarioController>();
}

CMario::~CMario()
{
}

void CMario::Awake()
{
	auto marioController = this->GetComponent<MarioController>();
	auto transform = this->GetComponent<MarioController>()->GetParentObject()->GetComponent<TransformComponent>();
	auto animation = this->GetComponent<MarioController>()->GetParentObject()->GetComponent<AnimationComponent>();
	auto velocity = this->GetComponent<MarioController>()->GetParentObject()->GetComponent<VelocityComponent>();

	//AddComponent<MarioController>()->Awake();
	marioController->Awake();
}

void CMario::Update(DWORD dt)
{
	CGameObject::Update(dt);
	if (marioController) {
		marioController->Update(dt);
	}
}

void CMario::Render()
{
	CGameObject::Render();
}
