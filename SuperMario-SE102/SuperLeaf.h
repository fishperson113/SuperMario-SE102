#pragma once
#include "GameObject.h"
#define SUPERLEAF_BBOX_WIDTH 16
#define SUPERLEAF_BBOX_HEIGHT 14
#define ID_ANI_SUPERLEAF 90001
#define LEAF_GRAVITY 0.00001f
// States
#define LEAF_STATE_FLY_UP 100
#define LEAF_STATE_FLOAT_DOWN 200

// Physics constants
#define LEAF_GRAVITY 0.00001f
#define LEAF_FLY_SPEED 0.2f
#define LEAF_FLOAT_SPEED_X 0.05f
#define LEAF_FLOAT_SPEED_Y 0.03f
class CSuperLeaf : public CGameObject
{
	bool isCollected;
private:
	float minY;  // Highest point the leaf can reach
	float minX;  // Leftmost boundary
	float maxX;
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	CSuperLeaf(float x, float y) : CGameObject(x, y)
	{
		isCollected = false;
		vx = 0.05f; 
		vy = 0.0f; 
		minY = y - 80.0f;  // Fly up to 80 pixels from starting position
		minX = x - 30.0f;  // 30 pixels to the left
		maxX = x + 30.0f;  // 30 pixels to the right

		// Initial state and velocity
		SetState(LEAF_STATE_FLY_UP);
	}
	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) { return 1; }
	void OnNoCollision(DWORD dt) override
	{
		x += vx * dt;
		y += vy * dt;
	}
	void BeCollected();
	bool IsCollected() const { return isCollected; }
	void SetCollected(bool collected) { isCollected = collected; }
	void OnCollisionWith(LPCOLLISIONEVENT e);
	void SetState(int state);

};

