#include "FallSensor.h"
#include"Koopas.h"
FallSensor::FallSensor(float x, float y, Koopas* owner, bool isFront) : CGameObject(x, y), owner(owner), isFront(isFront)
{
	this->ax = 0;
	this->ay = KOOPAS_GRAVITY;
	vx = 0;
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

	if (!owner) return;
}

void FallSensor::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void FallSensor::OnCollisionWith(LPCOLLISIONEVENT e)
{
	//if (!e->obj->IsBlocking()) return;
	//if (e->ny != 0 )
	//{
	//	vy = 0;
	//}
}
