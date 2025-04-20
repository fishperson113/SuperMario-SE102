#pragma once
#include "GameObject.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"
#include "DoorController.h"

class CDoor : public CGameObject
{
public:
	CDoor();
	~CDoor();

	void Awake();
	void Update(DWORD dt);
	void Render();
private:
	DoorController* doorController;
};

