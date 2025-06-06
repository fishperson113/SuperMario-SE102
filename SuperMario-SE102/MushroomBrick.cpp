#include "MushroomBrick.h"

void CMushroomBrick::Render()
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

void CMushroomBrick::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x - BRICK_BBOX_WIDTH / 2;
	t = y - BRICK_BBOX_HEIGHT / 2;
	r = l + BRICK_BBOX_WIDTH;
	b = t + BRICK_BBOX_HEIGHT;
}

void CMushroomBrick::SetState(int state)
{
	CGameObject::SetState(state);
}

void CMushroomBrick::SpawnMushroom()
{
	CMushroom* mushroom = new CMushroom(x, y - 32);
	mushroom->SetState(MUSHROOM_STATE_WALKING);

	CPlayScene* currentScene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	currentScene->GetObjectManager()->Add(mushroom);
}
