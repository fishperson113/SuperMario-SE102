#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "Mushroom.h"
#include "PlayScene.h"

#define ID_ANI_COINBRICK 10001
#define BRICK_WIDTH 16
#define BRICK_BBOX_WIDTH 16
#define BRICK_BBOX_HEIGHT 16

class CMushroomBrick : public CGameObject
{
private:
	bool isHitted;
public:
	CMushroomBrick(float x, float y, bool isHitted = false) : CGameObject(x, y) {
		this->isHitted = isHitted;
	}
	virtual void Render();
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	int IsCollidable() { return 0; }
	void SpawnMushroom();
};

