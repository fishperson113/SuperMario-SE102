#pragma once
#include "GameObject.h"
#define BRICK_SPEED_X	0.1f;
#define BRICK_SPEED_Y	0.31f
#define BRICK_PIECE_GRAVITY	0.001f

#define ID_ANI_BRICK_PIECE	10019
#define BRICK_PIECE_LIFE_TIME	2000 // 1 second
class BrickPiece : public CGameObject
{
	float ay;
	float start_time;
public:
	BrickPiece(float x, float y, float _vx, float _vy) : CGameObject(x, y)
	{
		this->vx = _vx;
		this->vy = -_vy;
		this->ay = BRICK_PIECE_GRAVITY;
		this->start_time = GetTickCount64();
	}
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void OnNoCollision(DWORD dt);
	void Render();
	int IsCollidable() { return 0; }
	void GetBoundingBox(float& l, float& t, float& r, float& b) {}
	int IsBlocking() { return 0; }
};

