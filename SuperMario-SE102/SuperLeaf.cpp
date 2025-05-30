#include "SuperLeaf.h"
#include "Mario.h"

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
	
	float time = (float)(GetTickCount64() % 1000) / 1000.0f; // Normalize time to [0, 1]
	originX = x; // Store the original x position
	x = originX + oscillationRange * sin(time * 2 * 3.14159f); // S

	CGameObject::Update(dt, coObjects);

}

void CSuperLeaf::Render()
{
	int aniId = ID_ANI_SUPERLEAF;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CSuperLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking()) return;

	if (dynamic_cast<CMario*>(e->obj))
	{
		CMario* mario = dynamic_cast<CMario*>(e->obj);
		if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		{
			mario->SetLevel(MARIO_LEVEL_BIG);
		}
		else {
			mario->SetLevel(MARIO_LEVEL_TAIL);
		}
	}
}