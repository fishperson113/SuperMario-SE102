#pragma once
#include "Component.h"
class KoopasController :public ScriptComponent
{
public:
	KoopasController() {};
	void Update(float dt) override;
	void Awake() override;
	void Start() override;
	void Render() override {};
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnNoCollision(DWORD dt) override {
		ScriptComponent::OnNoCollision(dt);
	}
};

