#include "Brick.h"
#include "PlayScene.h"
#include "BrickPiece.h"
#include"Coin.h"
void CBrick::Render()
{
	CAnimations* animations = CAnimations::GetInstance();
	if (isCoin)
	{
		animations->Get(ID_ANI_COIN)->Render(x, y);
	}
	else
	{
		animations->Get(this->aniId)->Render(x, y);
	}
}

void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x - this->bboxWidth / 2 + offsetX; 
	t = y - this->bboxHeight / 2 + offsetY;
	r = l + this->bboxWidth;
	b = t + this->bboxHeight;
}

int CBrick::IsDirectionColliable(float nx, float ny)
{
	if (this->aniId != 10000 && this->aniId != 10014 && this->aniId != 10015)
	{
		if (nx == 0 && ny == -1) return 1;
		else return 0;
	}
	else
		return 1;
}

void CBrick::SpawnBreakPiece()
{
	CPlayScene* current_scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();

	// Top-left piece
	BrickPiece* piece1 = new BrickPiece(x - 4.0f, y - 4.0f, -0.05f, 0.25f);

	// Bottom-left piece
	BrickPiece* piece2 = new BrickPiece(x - 4.0f, y + 4.0f, -0.04f, 0.2f);

	// Top-right piece
	BrickPiece* piece3 = new BrickPiece(x + 4.0f, y - 4.0f, 0.05f, 0.25f);

	// Bottom-right piece
	BrickPiece* piece4 = new BrickPiece(x + 4.0f, y + 4.0f, 0.04f, 0.2f);

	// Add the pieces to the scene
	current_scene->GetObjectManager()->Add(piece1);
	current_scene->GetObjectManager()->Add(piece2);
	current_scene->GetObjectManager()->Add(piece3);
	current_scene->GetObjectManager()->Add(piece4);
}

void CBrick::Break()
{
	if (!isBreakable)
	{
		return;
	}

	SetState(BRICK_STATE_HIT);
}

void CBrick::TurnToCoin()
{
	isCoin = true;
}

void CBrick::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	if (ableToChangeToCoin && !isCoin)
	{
		CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
		CMario* mario = (CMario*)scene->GetPlayer();

		if (mario && mario->GetIsHitSwitch())
		{
			TurnToCoin();
		}
	}
}
void CBrick::SetState(int state)
{
	this->state = state;

	switch (state)
	{
	case BRICK_STATE_NORMAL:
		break;

	case BRICK_STATE_HIT:
	{
		if (!isCoin)
		{
			SpawnBreakPiece();
		}
		if (isBreakable && !ableToChangeToCoin)
		{
			this->Delete();
		}
		else if (ableToChangeToCoin && !isCoin)
		{
			// If it can turn to coin, do so
			justTurnedToCoin = true;
			TurnToCoin();
		}
		break;
	}
	}
}