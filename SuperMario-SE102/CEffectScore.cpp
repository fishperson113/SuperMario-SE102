#include "CEffectScore.h"
#include "Animations.h"
#include "PlayScene.h"
#include "debug.h"

CEffectScore::CEffectScore(float x, float y, int score) : CEffect(x, y, EFFECT_SCORE_LIFETIME)
{
    this->score = score;
    this->animationId = ScoreToAnimationId(score);
    this->vy = -EFFECT_SCORE_RISE_SPEED;
    this->vx = 0.0f;
}

void CEffectScore::Render()
{
    CAnimations* animations = CAnimations::GetInstance();
    if (animations && animations->Get(animationId))
    {
        animations->Get(animationId)->Render(x, y);
    }
    else
    {
        DebugOut(L"[WARNING] Score effect animation ID %d not found!\n", animationId);
    }
}

void CEffectScore::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    CEffect::Update(dt, coObjects);
}

int CEffectScore::ScoreToAnimationId(int score)
{
    switch (score)
    {
    case SCORE_100:  return ID_ANI_EFFECT_SCORE_100;
    case SCORE_200:  return ID_ANI_EFFECT_SCORE_200;
    case SCORE_400:  return ID_ANI_EFFECT_SCORE_400;
    case SCORE_800:  return ID_ANI_EFFECT_SCORE_800;
    case SCORE_1000: return ID_ANI_EFFECT_SCORE_1000;
	case SCORE_LEVEL_UP: return ID_ANI_EFFECT_LEVEL_UP;
    default:         return ID_ANI_EFFECT_SCORE_100; 
    }
}