#include "SwitchBrick.h"
#include "Switch.h"

void CSwitchBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (this->state == SWITCH_BRICK_STATE_HIT)
	{
		animations->Get(10020)->Render(x, y);
	}
	else
	{
		animations->Get(10015)->Render(x, y);
	}
}

void CSwitchBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - SWITCH_BRICK_BBOX_WIDTH / 2;
	t = y - SWITCH_BRICK_BBOX_HEIGHT / 2;
	r = l + SWITCH_BRICK_BBOX_WIDTH;
	b = t + SWITCH_BRICK_BBOX_HEIGHT;
}

void CSwitchBrick::SetState(int state)
{
	CGameObject::SetState(state);
}

void CSwitchBrick::SpawnSwitch()
{
	CSwitch* sw = new CSwitch(x, y - 14);
	sw->SetState(SWITCH_STATE_NORMAL);
	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	currentScene->GetObjectManager()->Add(sw);
}
