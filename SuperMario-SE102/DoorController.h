#pragma once
#include "Component.h"
enum class DoorState {
	Closed,
	Opening,
	Open,
	Closing
};
class DoorController : public ScriptComponent
{
public:
	DoorController()
		: state(DoorState::Closed), doorAnimationId(700),animComponent(nullptr), collisionRange(30.0f) { }
	void Update(float dt) override;
	void Awake() override;
	void Start() override;
	void Render() override {};
private:
	void SetState(DoorState newState);
private:
	DoorState state;
	float collisionRange;
	int doorAnimationId ; 
	bool isOpen = false;        
	AnimationComponent* animComponent;
};

