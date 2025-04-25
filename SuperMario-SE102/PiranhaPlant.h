#pragma once
#include "GameObject.h"
#define PIRANHA_PLANT_BBOX_WIDTH 33
#define PIRANHA_PLANT_BBOX_HEIGHT 37
#define ID_ANI_PIRANHA_PLANT 200006
class CPiranhaPlant : public CGameObject
{
public:
	CPiranhaPlant(float x, float y) : CGameObject(x, y) {}
	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
};

