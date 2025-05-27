#include "SuperLeaf.h"

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SUPERLEAF_BBOX_WIDTH / 2;
	top = y - SUPERLEAF_BBOX_HEIGHT / 2;
	right = left + SUPERLEAF_BBOX_WIDTH;
	bottom = top + SUPERLEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	vy += LEAF_GRAVITY * dt;
	startTime = GetTickCount64();
	if (GetTickCount64() - startTime > 500)
	{
		vx += 0.001f * dt;
	}
	else
	{
		vx -= 0.001f * dt;
		startTime = GetTickCount64();
	}
	CGameObject::Update(dt, coObjects);

}

void CSuperLeaf::Render()
{
	int aniId = ID_ANI_SUPERLEAF;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
