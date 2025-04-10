#pragma once
#include "Component.h"
#include "AssetIDs.h"

class GoombaController : public ScriptComponent
{
public:
	GoombaController() : rightAnimId(ID_ANI_GOOMBA_WALKING), leftAnimId(ID_ANI_GOOMBA_WALKING), moveSpeed(50) {};
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

