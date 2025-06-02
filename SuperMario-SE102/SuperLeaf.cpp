#include "SuperLeaf.h"
#include "PlayScene.h"
#include "Mario.h"
#include"CEffectScore.h"
void CSuperLeaf::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - SUPERLEAF_BBOX_WIDTH / 2;
	top = y - SUPERLEAF_BBOX_HEIGHT / 2;
	right = left + SUPERLEAF_BBOX_WIDTH;
	bottom = top + SUPERLEAF_BBOX_HEIGHT;
}

void CSuperLeaf::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	// Apply gravity if needed
	vy += LEAF_GRAVITY * dt;

	// Determine flight behavior based on state
	if (state == LEAF_STATE_FLY_UP)
	{
		// If flying up from brick
		if (y <= minY)
		{
			// Switch to floating down when reaching min height
			SetState(LEAF_STATE_FLOAT_DOWN);
		}
	}
	else if (state == LEAF_STATE_FLOAT_DOWN)
	{
		// Oscillate left and right during floating
		if (x >= maxX)
		{
			vx = -LEAF_FLOAT_SPEED_X;
		}
		else if (x <= minX)
		{
			vx = LEAF_FLOAT_SPEED_X;
		}

		// Control the falling speed
		vy = LEAF_FLOAT_SPEED_Y;
	}

	CGameObject::Update(dt, coObjects);

}

void CSuperLeaf::Render()
{
	int aniId = ID_ANI_SUPERLEAF;
	CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}
void CSuperLeaf::OnCollisionWith(LPCOLLISIONEVENT e)
{
	if (isCollected) return;
	if (e->obj->IsBlocking()) return;
	if (dynamic_cast<CMario*>(e->obj))
	{
		BeCollected();
	}
	
}
void CSuperLeaf::BeCollected()
{
	CEffectScore* levelUpEffect = new CEffectScore(x, y, SCORE_LEVEL_UP);

	CPlayScene* scene = (CPlayScene*)CGame::GetInstance()->GetCurrentScene();
	if (scene) {
		scene->GetObjectManager()->Add(levelUpEffect);
	}
	CMario* mario = NULL;
	if (scene)
	{
		mario = dynamic_cast<CMario*>(scene->GetPlayer());
	}
	isCollected = true;
	// Add points to Mario's score
	mario->AddPoints(1000);
	if (mario->GetLevel() == MARIO_LEVEL_SMALL)
		mario->SetLevel(MARIO_LEVEL_BIG);
	else if (mario->GetLevel() == MARIO_LEVEL_BIG)
		mario->SetLevel(MARIO_LEVEL_TAIL);
	Delete();
}
void CSuperLeaf::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case LEAF_STATE_FLY_UP:
		vy = -LEAF_FLY_SPEED;
		vx = 0;
		break;

	case LEAF_STATE_FLOAT_DOWN:
		vy = LEAF_FLOAT_SPEED_Y;
		vx = LEAF_FLOAT_SPEED_X;
		break;
	}
}