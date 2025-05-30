#pragma once
#include "GameObject.h"
class Koopas;
class FallSensor : public CGameObject
{
private:
    Koopas* owner;
    float ax;
    float ay;
public:
    FallSensor(float x, float y, Koopas* owner);

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    virtual void Render()
    {
         //RenderBoundingBox();
    }

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);

    virtual int IsCollidable() { return 1; }  
    virtual int IsBlocking() { return 0; }    
    virtual void OnNoCollision(DWORD dt);

    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

