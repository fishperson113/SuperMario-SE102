#pragma once
#include "GameObject.h"

#define BOOMERANG_BRO_GRAVITY 0.002f
#define BOOMERANG_BRO_WALKING_SPEED 0.05f

#define BOOMERANG_BRO_BBOX_WIDTH 15
#define BOOMERANG_BRO_BBOX_HEIGHT 23
#define BOOMERANG_BRO_BBOX_HEIGHT_DIE 9

#define BOOMERANG_BRO_DIE_TIMEOUT 500
#define BOOMERANG_BRO_THROW_COOLDOWN 2000

#define BOOMERANG_BRO_STATE_WALKING 100
#define BOOMERANG_BRO_STATE_IDLE 200
#define BOOMERANG_BRO_STATE_THROWING 300
#define BOOMERANG_BRO_STATE_DIE_TOP 400
#define BOOMERANG_BRO_STATE_DIE_BOTTOM 500

#define ID_ANI_BOOMERANG_BRO_WALKING_LEFT 58000
#define ID_ANI_BOOMERANG_BRO_WALKING_RIGHT 58001
#define ID_ANI_BOOMERANG_BRO_THROW_LEFT 58002
#define ID_ANI_BOOMERANG_BRO_THROW_RIGHT 58003
#define ID_ANI_BOOMERANG_BRO_IDLE_LEFT 58004
#define ID_ANI_BOOMERANG_BRO_IDLE_RIGHT 58005
#define ID_ANI_BOOMERANG_BRO_DIE_TOP 58006
#define ID_ANI_BOOMERANG_BRO_DIE_BOTTOM 58007
class CBoomerang; // Forward declaration
class CBoomerangBro : public CGameObject
{
protected:
    float ax;
    float ay;

    ULONGLONG die_start;
    ULONGLONG walk_start;
    ULONGLONG throw_start;
    ULONGLONG throw_cooldown;

    int direction; 

    bool hasBoomerang;
    CBoomerang* boomerang;
    void ThrowBoomerang();
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }
    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
    CBoomerangBro(float x, float y);
	CBoomerang* GetBoomerang() { return boomerang; }
    virtual ~CBoomerangBro();
    virtual void SetState(int state);
};