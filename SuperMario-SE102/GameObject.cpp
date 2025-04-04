#include <d3dx9.h>

#include "debug.h"
#include "Game.h"
#include "GameObject.h"
#include"Component.h"

/*
	Initialize game object 
*/
CGameObject::CGameObject()
{
	active = true;
}

void CGameObject::Update(DWORD dt)
{
	if (!active) return;

	for (auto component : components)
	{
		if (component->IsEnabled())
			component->Update(dt);
	}
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

