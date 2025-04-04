#pragma once
#include "Component.h"
class MarioController : public ScriptComponent
{
public:
	MarioController() : rightAnimId(500), leftAnimId(501), moveSpeed(100) {};
	void Update(float dt) override;
	void Awake() override;
	void Start() override;
	void Render() override {};
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
private:
	int rightAnimId;
	int leftAnimId;
	float moveSpeed;
};

