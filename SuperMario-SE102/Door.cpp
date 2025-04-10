#include "Door.h"
#include "AssetIDs.h"

CDoor::CDoor()
{
	AddComponent<DoorController>();
	AddComponent<TransformComponent>();
	AddComponent<AnimationComponent>();
}

CDoor::~CDoor()
{
}

void CDoor::Awake()
{
	auto doorController = this->GetComponent<DoorController>();
	auto transform = this->GetComponent<DoorController>()->GetParentObject()->GetComponent<TransformComponent>();
	auto animation = this->GetComponent<DoorController>()->GetParentObject()->GetComponent<AnimationComponent>();

	doorController->Awake();
}

void CDoor::Update(DWORD dt)
{
	CGameObject::Update(dt);
	if (doorController) {
		doorController->Update(dt);
	}
}

void CDoor::Render()
{
	CGameObject::Render();
}
