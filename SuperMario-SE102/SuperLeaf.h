#pragma once
#include "GameObject.h"
#define SUPERLEAF_BBOX_WIDTH 16
#define SUPERLEAF_BBOX_HEIGHT 14
#define ID_ANI_SUPERLEAF 90001

class CSuperLeaf : public CGameObject
{
public:
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Update(DWORD dt) {}
	void Render();
	CSuperLeaf(float x, float y) : CGameObject(x, y) {}
};

