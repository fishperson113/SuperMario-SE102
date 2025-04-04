#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define GOOMBA_WIDTH 10
#define GOOMBA_BBOX_WIDTH 10
#define GOOMBA_BBOX_HEIGHT 16

class CGoomba : public CGameObject
{
public:
	CGoomba();
	~CGoomba();

	void Update(DWORD dt);
	void Render();
	void Start();
	void Awake();
private:
	void Move();
};

