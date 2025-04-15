#include "Portal.h"

CPortal::CPortal(float l, float t, float r, float b, int scene_id)
{
	DebugOut(L"[INFO] Portal object has been created!\n");
	this->active = true;

	this->scene_id = scene_id;
	this->width = r - l;
	this->height = b - t;

	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(l, t);

	auto collider = AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, 25, 25);

	auto animation = AddComponent<AnimationComponent>();
	LPANIMATION ani = CAnimations::GetInstance()->Get(ID_ANI_PORTAL);
	animation->SetCurrentAnimation(ani);
}

CPortal::~CPortal()
{
}
