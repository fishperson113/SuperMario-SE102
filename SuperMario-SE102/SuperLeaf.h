#pragma once
#include "GameObject.h"
#define SUPERLEAF_BBOX_WIDTH 16
#define SUPERLEAF_BBOX_HEIGHT 14
#define ID_ANI_SUPERLEAF 90001
#define LEAF_GRAVITY 0.001f

class CSuperLeaf : public CGameObject
{
private:
	ULONGLONG startTime = 0; // Time when the leaf was created
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	CSuperLeaf(float x, float y) : CGameObject(x, y) {}
	int IsCollidable() override { return 0; }
	int IsBlocking() override { return 0; }
	int IsDirectionColliable(float nx, float ny) { return 1; }
};

