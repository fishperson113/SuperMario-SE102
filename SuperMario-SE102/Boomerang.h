#pragma once
#include "GameObject.h"

#define BOOMERANG_GRAVITY 0.0002f
#define BOOMERANG_SPEED_X 0.15f
#define BOOMERANG_SPEED_Y 0.1f
#define BOOMERANG_SPEED_DIFF 1.5f
#define BOOMERANG_RETURN_DISTANCE 100.0f

#define BOOMERANG_BBOX_WIDTH 15
#define BOOMERANG_BBOX_HEIGHT 15

#define BOOMERANG_STATE_INACTIVE 0
#define BOOMERANG_STATE_1 1  // Moving up and outward
#define BOOMERANG_STATE_2 2  // Moving horizontal
#define BOOMERANG_STATE_3 3  // Moving down and outward
#define BOOMERANG_STATE_4 4  // Moving down faster (apex of throw)
#define BOOMERANG_STATE_5 5  // Moving down and toward thrower
#define BOOMERANG_STATE_6 6  // Moving horizontal toward thrower

#define BOOMERANG_CHANGE_STATE_TIME 300
#define BOOMERANG_DIFF 2

#define ID_ANI_BOOMERANG_STATIC 57000
#define ID_ANI_BOOMERANG_SPINNING 57001

class CBoomerang : public CGameObject
{
protected:
    float maxDistance;
    CGameObject* owner;
    int phase;               // 0: going, 1: returning
    DWORD state_start;       // Timer for state changes
    int dir;                 // Direction: 1=right, -1=left

    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render();

    virtual int IsCollidable() { return 1; };
    virtual int IsBlocking() { return 0; }
    virtual void OnNoCollision(DWORD dt);
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);

public:
    CBoomerang(float x, float y, CGameObject* owner = NULL);
    virtual void SetState(int state);
    void SetDirection(int dir);
    void StartChangeState();
    bool IsReturning();
};