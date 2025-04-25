#include "PiranhaPlant.h"

void CPiranhaPlant::Render()
{
	int aniId = ID_ANI_PIRANHA_PLANT;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CPiranhaPlant::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - PIRANHA_PLANT_BBOX_WIDTH / 2;
	t = y - PIRANHA_PLANT_BBOX_HEIGHT / 2;
	r = l + PIRANHA_PLANT_BBOX_WIDTH;
	b = t + PIRANHA_PLANT_BBOX_HEIGHT;
}
