#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include"Component.h"

/*
	Initialize game object 
*/
extern vector<LPGAMEOBJECT>* coObjects;
CGameObject::CGameObject()
{
	active = true;
}

void CGameObject::Update(DWORD dt)
{
	for (auto component : components)
	{
		component->Update(dt);
	}

	CCollision::GetInstance()->Process(this, dt, coObjects);
}

void CGameObject::Render()
{
	if (!active) return;
	auto transform = GetComponent<TransformComponent>();
	if (transform == nullptr) return;

	auto collider = GetComponent<ColliderComponent>();
	if (collider) {
		collider->Render();
	}
	auto animComponent = GetComponent<AnimationComponent>();
	if (animComponent) {
		animComponent->Render();
		return;
	}
	auto spriteComponent = GetComponent<SpriteComponent>();
	if (spriteComponent) {
		spriteComponent->Render();
	}
}

CGameObject::~CGameObject()
{
}

