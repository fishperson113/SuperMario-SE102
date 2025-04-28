#pragma once
#include "GameObject.h"
#include "SuperLeaf.h"
#include "PlayScene.h"
#include "Game.h"

class CSuperLeafBrick : public CGameObject
{
public:
	CSuperLeafBrick(float x, float y) : CGameObject(x, y) {}
	virtual void Render();
	virtual void Update(DWORD dt) {}
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	virtual void SetState(int state);
	void SpawnSuperLeaf();
};

