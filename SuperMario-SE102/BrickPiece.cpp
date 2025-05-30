#include "BrickPiece.h"
#include "PlayScene.h"

void BrickPiece::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	//if(GetTickCount64() - start_time > BRICK_PIECE_LIFE_TIME)
	//{
	//	//isDeleted = true;
	//	return;
	//}
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