#pragma once
#include "GameObject.h"

#define HITBOX_WIDTH 24
#define HITBOX_HEIGHT 16
#define HITBOX_LIFESPAN 500
class HitBox :  public CGameObject
{
protected:
    ULONGLONG activate_start;
    CGameObject* owner;  // The owner of this hitbox (Mario)
    bool isActive;

public:
    HitBox(CGameObject* owner);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    void Activate();
    bool IsActive() { return isActive; }

    int IsCollidable() { return isActive; }
    int IsBlocking() { return 0; }

    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
};

