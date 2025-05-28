#include "Switch.h"

void CSwitch::Update(DWORD dt)
{
}

void CSwitch::Render()
{
	if (state == SWITCH_STATE_NORMAL)
		CAnimations::GetInstance()->Get(ANI_SWITCH)->Render(x, y);
	else
		CAnimations::GetInstance()->Get(ANI_SWITCH_HITTED)->Render(x, y);
}

void CSwitch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	if (state == SWITCH_STATE_NORMAL)
	{
		l = x - SWITCH_BBOX_WIDTH / 2;
		t = y - SWITCH_BBOX_HEIGHT / 2;
		r = l + SWITCH_BBOX_WIDTH;
		b = t + SWITCH_BBOX_HEIGHT;
	}
	else
	{

	}
}

void CSwitch::SetState(int state)
{
	switch (state)
	{
	case SWITCH_STATE_HITTED:
		y += 6.0f;

		break;
	}

	CGameObject::SetState(state);
}
