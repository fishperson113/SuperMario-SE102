#pragma once
#include "Component.h"
class MarioController : public ScriptComponent
{
public:
	MarioController() : rightAnimId(500), leftAnimId(501), moveSpeed(100), isOnGround(false) {};
	void Update(float dt) override;
	void Awake() override;
	void Start() override;
	void Render() override {};
	void OnCollisionWith(LPCOLLISIONEVENT e) override;
	void OnNoCollision(DWORD dt) override {
		ScriptComponent::OnNoCollision(dt);
	}
public:
	void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
	void OnCollisionWithCoin(LPCOLLISIONEVENT e);
	void OnCollisionWithPortal(LPCOLLISIONEVENT e);
	void OnCollisionWithPlatform(LPCOLLISIONEVENT e);
	void SetCanJump(bool canJump);
private:
	int rightAnimId;
	int leftAnimId;
	float moveSpeed;
	float isOnGround;
	int lastDirection = 0;
	bool canJump = true;
	float maxVy = 1.0f;
};

