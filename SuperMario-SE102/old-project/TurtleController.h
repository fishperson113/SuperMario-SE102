#pragma once
#include "Component.h"
#include "AssetIDs.h"

class TurtleController : public ScriptComponent
{
public:
	TurtleController() : rightAnimId(ID_ANI_TURTLE), leftAnimId(ID_ANI_TURTLE), moveSpeed(50) {};
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

