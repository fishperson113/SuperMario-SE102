#include "Coin.h"

void CCoin::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_COIN)->Render(x, y);

	//RenderBoundingBox();
}

void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);
	x += vx * dt;
	y += vy * dt;

	if (this->state == COIN_STATE_JUMP && GetTickCount64() - spawnTime > COIN_LIFETIME)
	{
		this->Delete(); // Remove the coin from the game world
	}
	if (this->state == COIN_STATE_JUMP && GetTickCount64() - spawnTime > COIN_JUMPTIME)
		vy += COIN_GRAVITY * dt;
}

void CCoin::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - COIN_BBOX_WIDTH / 2;
	t = y - COIN_BBOX_HEIGHT / 2;
	r = l + COIN_BBOX_WIDTH;
	b = t + COIN_BBOX_HEIGHT;
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == COIN_STATE_JUMP)
		vy = -COIN_JUMP_SPEED; // Set initial upward velocity
}
