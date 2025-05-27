#include "Boomerang.h"
#include "debug.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScene.h"
#include <cmath>

// Define PI if not available
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#define BOOMERANG_STATE_RETURNING 2
CBoomerang::CBoomerang(float x, float y, CGameObject* owner) : CGameObject(x, y)
{
    this->owner = owner;
    this->phase = 0;
    this->state_start = 0;
    this->dir = 1;

    SetState(BOOMERANG_STATE_INACTIVE);
}

CBoomerang::~CBoomerang()
{
    owner = nullptr; 

    DebugOut(L"[INFO] Boomerang destroyed\n");
}

void CBoomerang::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    left = x - BOOMERANG_BBOX_WIDTH / 2;
    top = y - BOOMERANG_BBOX_HEIGHT / 2;
    right = left + BOOMERANG_BBOX_WIDTH;
    bottom = top + BOOMERANG_BBOX_HEIGHT;
}

void CBoomerang::OnNoCollision(DWORD dt)
{
    // Movement is handled in Update
}

void CBoomerang::OnCollisionWith(LPCOLLISIONEVENT e)
{
   
}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    x += vx * dt;
    y += vy * dt;

    if (state_start != 0)
        state_start += dt;

    // Handle state transitions
    if (state == BOOMERANG_STATE_1 && state_start >= BOOMERANG_CHANGE_STATE_TIME)
    {
        state_start = 1;
        SetState(BOOMERANG_STATE_2);
    }
    else if (state == BOOMERANG_STATE_2 && state_start >= BOOMERANG_CHANGE_STATE_TIME / BOOMERANG_DIFF)
    {
        state_start = 1;
        SetState(BOOMERANG_STATE_3);
    }
    else if (state == BOOMERANG_STATE_3 && state_start >= BOOMERANG_CHANGE_STATE_TIME / BOOMERANG_DIFF)
    {
        state_start = 1;
        SetState(BOOMERANG_STATE_4);
    }
    else if (state == BOOMERANG_STATE_4 && state_start >= BOOMERANG_CHANGE_STATE_TIME / BOOMERANG_DIFF)
    {
        state_start = 1;
        SetState(BOOMERANG_STATE_5);
    }
    else if (state == BOOMERANG_STATE_5 && state_start >= BOOMERANG_CHANGE_STATE_TIME / BOOMERANG_DIFF)
    {
        state_start = 1;
        SetState(BOOMERANG_STATE_6);
    }
    else if (state == BOOMERANG_STATE_6 && state_start >= BOOMERANG_CHANGE_STATE_TIME / BOOMERANG_DIFF)
    {
        SetActive(false);
        SetState(BOOMERANG_STATE_INACTIVE);
    }
}

void CBoomerang::Render()
{
    if (!IsActive()) return;

    int aniId;
    if (dir > 0)
    {
        if (state == BOOMERANG_STATE_INACTIVE)
            aniId = ID_ANI_BOOMERANG_STATIC;
        else
            aniId = ID_ANI_BOOMERANG_SPINNING;
    }
    else
    {
        if (state == BOOMERANG_STATE_INACTIVE)
            aniId = ID_ANI_BOOMERANG_STATIC;
        else
            aniId = ID_ANI_BOOMERANG_SPINNING;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
}

void CBoomerang::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case BOOMERANG_STATE_INACTIVE:
        vx = vy = 0;
        break;

    case BOOMERANG_STATE_1:  // Moving up and outward
        vx = dir * BOOMERANG_SPEED_X;
        vy = -BOOMERANG_SPEED_Y;
        StartChangeState();
        phase = 0;
        break;

    case BOOMERANG_STATE_2:  // Moving horizontal
        vy = 0;
        phase = 0;
        break;

    case BOOMERANG_STATE_3:  // Moving down and outward
        vy = BOOMERANG_SPEED_Y;
        phase = 0;
        break;

    case BOOMERANG_STATE_4:  // Moving down faster (apex of throw)
        vx = 0;
        vy = BOOMERANG_SPEED_Y * BOOMERANG_SPEED_DIFF;
        phase = 1;  // Now returning
        break;

    case BOOMERANG_STATE_5:  // Moving down and toward thrower
        vy = BOOMERANG_SPEED_Y;
        vx = -dir * BOOMERANG_SPEED_X;
        phase = 1;
        break;

    case BOOMERANG_STATE_6:  // Moving horizontal toward thrower
        vx = -dir * BOOMERANG_SPEED_X;
        vy = 0;
        phase = 1;
        break;
    }
}

void CBoomerang::SetDirection(int dir)
{
    this->dir = dir;
}
void CBoomerang::StartChangeState()
{
    state_start = 1;
}

bool CBoomerang::IsReturning()
{
    return phase == 1;
}
