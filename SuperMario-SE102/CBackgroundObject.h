#pragma once
#include "GameObject.h"
#include "Sprites.h"
class CBackgroundObject :public CGameObject
{
private:
    int spriteId;
    int objectCount; 
    float offsetX;   
    float offsetY;   

public:
    CBackgroundObject(float x, float y, int spriteId, int objectCount = 1, float offsetX = 0.0f, float offsetY = 0.0f);
    virtual void Render();
    virtual void Update(DWORD dt) {} 
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {} 

};

