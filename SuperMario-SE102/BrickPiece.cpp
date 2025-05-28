#include "BrickPiece.h"
#include "PlayScene.h"

void BrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += ay * dt;
}

void BrickPiece::OnNoCollision(DWORD dt)
{
	x += vx * dt;
	y += vy * dt;
}

void BrickPiece::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	animations->Get(ID_ANI_BRICK_PIECE)->Render(x, y);
}