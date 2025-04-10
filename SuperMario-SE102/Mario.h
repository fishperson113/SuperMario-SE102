#pragma once
#include "GameObject.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"
#include "MarioController.h"

class CMario : public CGameObject
{
public:
    CMario();
    ~CMario();

    void Awake();
    void Update(DWORD dt);
    void Render();
private:
    MarioController* marioController;
};

