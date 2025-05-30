#include "Mushroom.h"
#include "PlayScene.h"
#include "Mario.h"
void CMushroom::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - MUSHROOM_BBOX_WIDTH / 2;
	top = y - MUSHROOM_BBOX_HEIGHT / 2;
	right = left + MUSHROOM_BBOX_WIDTH;
	bottom = top + MUSHROOM_BBOX_HEIGHT;
}

void CMushroom::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;

	CGameObject::Update(dt, coObjects);
}

void CMushroom::Render()
{
	int aniId = ID_ANI_MUSHROOM_WALKING;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CMushroom::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void CMushroom::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (!e->obj->IsBlocking()) return;

	if (e->ny != 0) // Collision with the ground or ceiling
	{
		vy = 0; // Stop vertical movement
	}
	else if (e->nx != 0) // Collision with a wall
	{
		vx = -vx; // Reverse horizontal direction
	}
}

CMushroom::CMushroom(float x, float y)
{
	this->x = x;
	this->y = y;
	this->ax = 0;
	this->ay = MUSHROOM_GRAVITY;
	SetState(MUSHROOM_STATE_WALKING);
}

void CMushroom::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MUSHROOM_STATE_WALKING:
		vx = MUSHROOM_WALKING_SPEED; // Set initial horizontal velocity
		break;
	}
}
