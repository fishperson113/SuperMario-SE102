#pragma once
#include "GameObject.h"
#include "Sprites.h"
class CBackgroundObject :public CGameObject
{
private:
    int spriteId;

public:
    CBackgroundObject(float x, float y, int spriteId);
    virtual void Render();
    virtual void Update(DWORD dt) {} 
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {} 

};

