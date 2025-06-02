#include "CoinBrick.h"
#include "Koopas.h"

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

void CCoinBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (this->breakCount == 0)
		this->SetState(BRICK_STATE_HIT);
	if (this->state == BRICK_STATE_HITTING)
	{
		if (this->y != this->min_pos)
			this->y += 4;
	}
	if (GetTickCount64() - this->jump_start > 100)
	{
		this->y = old_pos;
		if (this->state != BRICK_STATE_HIT)
		{
			this->SetState(BRICK_STATE_NORMAL);
		}
		jump_start = GetTickCount64();
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

void CCoinBrick::Bounce()
{
	jump_start = GetTickCount64();
	if (this->breakCount != 0 && this->state != BRICK_STATE_HIT)
	{
		this->y -= 4;
	}
}

void CCoinBrick::OnNoCollision(DWORD dt)
{
	y += vy * dt;
}

void CCoinBrick::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (e->obj->IsBlocking())
	{
		if (e->ny != 0)
		{
			vy = 0;
		}
		else if (e->nx != 0)
		{
			vx = -vx;
		}
	}

	if (dynamic_cast<Koopas*>(e->obj))
		OnCollisionWithKoopas(e);

}

void CCoinBrick::OnCollisionWithKoopas(LPCOLLISIONEVENT e)
{
	Koopas* koopas = (Koopas*)e->obj;

	if (e->ny > 0) // Collision from below
	{
		if (this->state == BRICK_STATE_HITTING)
		{
			float x, y;
			koopas->GetPosition(x, y);
			koopas->SetPosition(x, y - 16);
			if (koopas->GetState() == KOOPAS_STATE_WALKING)
				koopas->SetState(KOOPAS_STATE_SHELL);
			else if (koopas->GetState() == KOOPAS_STATE_SHELL)
				koopas->SetState(KOOPAS_STATE_UPSIDE_DOWN);
			else if (koopas->GetState() == KOOPAS_STATE_UPSIDE_DOWN)
				koopas->SetState(KOOPAS_STATE_DIE);
		}
	}
}
