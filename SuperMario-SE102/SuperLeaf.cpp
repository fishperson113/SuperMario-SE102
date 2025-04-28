#include "SuperLeaf.h"

void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SUPERLEAF_BBOX_WIDTH / 2;
	top = y - SUPERLEAF_BBOX_HEIGHT / 2;
	right = left + SUPERLEAF_BBOX_WIDTH;
	bottom = top + SUPERLEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Render()
{
	int aniId = ID_ANI_SUPERLEAF;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
