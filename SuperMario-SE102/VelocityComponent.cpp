#include"Component.h"
#include"Game.h"

void VelocityComponent::Update(float dt)
{
	if (useGravity && !isOnGround) {
		velY += gravityAccel * dt;

	/*	if (velY > maxFallingSpeed) {
			velY = maxFallingSpeed;
		}*/
	}
}

void VelocityComponent::UpdatePosition(float dt)
{
	auto transform = parentObject->GetComponent<TransformComponent>();
	if (transform)
	{
		float newX = transform->GetPositionX() + velX * speed * dt;
		float newY = transform->GetPositionY() + velY * speed * dt;
		transform->SetPosition(newX, newY);
	}

}

void VelocityComponent::MoveToPosition(float x, float y)
{
	auto transform = parentObject->GetComponent<TransformComponent>();
	if (transform)
	{
		transform->SetPosition(x, y);
	}
}

void VelocityComponent::EnableGravity(bool enable)
{
	useGravity = enable;
}

void VelocityComponent::SetGravityAcceleration(float accel)
{
	gravityAccel = accel;
}

void VelocityComponent::SetMaxFallingSpeed(float maxSpeed)
{
	maxFallingSpeed = maxSpeed;
}

void VelocityComponent::SetGrounded(bool grounded)
{
	isOnGround = grounded;
	if (grounded && velY > 0) {
		velY = 0; 
	}
}

bool VelocityComponent::IsGrounded() const
{
	return isOnGround;
}
