#include "FallSensor.h"
#include"Koopas.h"
FallSensor::FallSensor(float x, float y, Koopas* owner) : CGameObject(x, y), owner(owner)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	vx = 0;
	vy = 0;
}
void FallSensor::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - 4;
    top = y - 4;
    right = x + 4;
    bottom = y + 4;
}

void FallSensor::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
    if (owner)
    {
        float ownerX, ownerY;
        owner->GetPosition(ownerX, ownerY);

        float ownerVx = 0;
        owner->GetSpeed(ownerVx, vy);

        if (ownerVx > 0) // Koopas moving right
        {
            x = ownerX + KOOPAS_BBOX_WIDTH / 2 + 4;
        }
        else // Koopas moving left or still
        {
            x = ownerX - KOOPAS_BBOX_WIDTH / 2 - 4; 
        }
    }
}

void FallSensor::OnNoCollision(DWORD dt)
{
	y += vy * dt;
    if (owner)
    {
        owner->ChangeDirection();
    }
}

void FallSensor::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking() && e->ny < 0)
	{
		vy = 0;
	}
}
