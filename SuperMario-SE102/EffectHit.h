#pragma once
#include "CEffect.h"
#define EFF_COL_TYPE_NORMAL 0 
#define EFF_COL_TYPE_SMOKE_EVOLVE 1 

#define ANI_ID_EFFECT_COLLISION_NORMAL 15
#define ANI_ID_EFFECT_COLLISION_SMOKE 21

#define EFFECT_COLLISION_NORMAL_TIME_OUT 180
#define EFFECT_COLLISION_SMOKE_TIME_OUT 750

#define EFFECT_COLLISION_STATE_RELEASE	0
#define EFFECT_COLLISION_STATE_NEED_TRIGGER	1
class CEffectHit :public CEffect
{
	ULONGLONG effect_start;
	int type;
public:
	CEffectHit(float x, float y, ULONGLONG eff_col_start, int type) :CEffect(x, y)
	{
		this->effect_start = -1;
		this->type = type;
		this->effect_start = eff_col_start;
	};

	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void Render();
	void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	int GetAniEffect();
};

