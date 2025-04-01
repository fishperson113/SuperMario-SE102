#include"Component.h"
#include"Game.h"

void VelocityComponent::Update(float dt) 
{
	auto transform = parentObject->GetComponent<TransformComponent>();
	if (transform)
	{
		float dtSeconds = dt / 1000.0f;
		float newX = transform->GetPositionX() + velX * speed * dtSeconds;
		float newY = transform->GetPositionY() + velY * speed * dtSeconds;
		transform->SetPosition(newX, newY);
	}

}