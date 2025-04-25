#include "Bullet.h"

CBullet::CBullet(float x, float y)
{
	this->x = x;
	this->y = y;
}

void CBullet::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - BULLET_BBOX_WIDTH / 2;
	top = y - BULLET_BBOX_HEIGHT / 2;
	right = left + BULLET_BBOX_WIDTH;
	bottom = top + BULLET_BBOX_HEIGHT;

}

void CBullet::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
	vx += ax * dt;
}

void CBullet::Render()
{
	int aniId = ID_ANI_BULLET_FLYING;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CBullet::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

