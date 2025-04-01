#pragma once

#include <Windows.h>
#include <d3dx10.h>
#include <vector>
#include "Texture.h"
#include "Animation.h"
#include "Animations.h"
class Component;

class CGameObject
{
protected: 
	std::vector<Component*> components;
	bool active;
public: 

	CGameObject();
	void Update(DWORD dt);
	void Render();

	void Activate() { active = true; }
	void Deactivate() { active = false; }
	bool IsActive() const { return active; }
	~CGameObject();
public:
	template<typename T>
	T* AddComponent() {
		static_assert(std::is_base_of<Component, T>::value, "T must inherit from Component");
		auto component = new T();
		component->SetParentObject(this);
		components.push_back(component);
		return component;
	}

	template<typename T>
	T* GetComponent() {
		for (auto component : components) {
			if (T* result = dynamic_cast<T*>(component)) {
				return result;
			}
		}
		return nullptr;
	}
};

typedef CGameObject * LPGAMEOBJECT;