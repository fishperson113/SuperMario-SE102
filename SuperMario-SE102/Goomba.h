#pragma once
#include "GameObject.h"
#include "Component.h"
#include "GoombaController.h"
#include "debug.h"
#include "Game.h"

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
	void Awake();
private:
	GoombaController* goombaController;
};

