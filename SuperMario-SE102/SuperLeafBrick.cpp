#include "SuperLeafBrick.h"
#define ID_ANI_COINBRICK 10001
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16
#define BRICK_STATE_HIT 100
void CSuperLeafBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (this->state == BRICK_STATE_HIT)
	{
		animations->Get(ID_ANI_COINBRICK + 1)->Render(x, y);
	}
	else
	{
		animations->Get(ID_ANI_COINBRICK)->Render(x, y);
	}
}

void CSuperLeafBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CSuperLeafBrick::SetState(int state)
{
	CGameObject::SetState(state);
}

void CSuperLeafBrick::SpawnSuperLeaf()
{
	CSuperLeaf* superLeaf = new CSuperLeaf(x+BRICK_BBOX_WIDTH, y - BRICK_BBOX_HEIGHT);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	currentScene->GetObjectManager()->Add(superLeaf);
}
