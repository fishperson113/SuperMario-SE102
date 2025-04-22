#include "CoinBrick.h"

void CCoinBrick::Render()
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

void CCoinBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CCoinBrick::SetState(int state)
{
	CGameObject::SetState(state);
}

void CCoinBrick::SpawnCoin()
{
	CCoin* coin = new CCoin(x, y - BRICK_BBOX_HEIGHT);
	coin->SetState(COIN_STATE_JUMP);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	currentScene->GetObjectManager()->Add(coin);
}
