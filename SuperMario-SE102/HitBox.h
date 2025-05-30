#pragma once
#include "GameObject.h"

#define HITBOX_WIDTH 24
#define HITBOX_HEIGHT 16
#define HITBOX_LIFESPAN 400
class HitBox :  public CGameObject
{
    void OnCollisionWithGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithParaGoomba(LPCOLLISIONEVENT e);
    void OnCollisionWithKoopas(LPCOLLISIONEVENT e);
    void OnCollisionWithKoopaParatroopa(LPCOLLISIONEVENT e);
    void OnCollisionWithPiranhaPlant(LPCOLLISIONEVENT e);
    void OnCollisionWithBullet(LPCOLLISIONEVENT e);
    void OnCollisionWithBrick(LPCOLLISIONEVENT e);
	void OnCollisionWithSwitchBrick(LPCOLLISIONEVENT e);
protected:
    ULONGLONG activate_start;
    CGameObject* owner;  // The owner of this hitbox (Mario)
    bool isActivate;

public:
    HitBox(CGameObject* owner);
    ~HitBox() { owner = nullptr; };
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    void Activate();
    bool IsActivate() { return isActivate; }

    int IsCollidable() { return 1; }
    int IsBlocking() { return 0; }

    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    void OnNoCollision(DWORD dt);
    void CheckOverlaps(vector<LPGAMEOBJECT>* coObjects);
};

