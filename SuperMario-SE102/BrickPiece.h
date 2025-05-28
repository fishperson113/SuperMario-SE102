#pragma once
#include "GameObject.h"
#define BRICK_SPEED_X	0.1f;
#define BRICK_SPEED_Y	0.31f
#define BRICK_PIECE_GRAVITY	0.001f

#define ID_ANI_BRICK_PIECE	10019
class BrickPiece : public CGameObject
{
	float ay;
public:
	BrickPiece(float x, float y, float _vx, float _vy) : CGameObject(x, y)
	{
		this->vx = _vx;
		this->vy = -_vy;
		this->ay = BRICK_PIECE_GRAVITY;
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnNoCollision(DWORD dt);
	void Render();
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	int IsBlocking() { return 0; }
};

