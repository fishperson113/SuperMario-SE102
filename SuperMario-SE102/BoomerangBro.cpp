#include "BoomerangBro.h"
#include "debug.h"
#include "Brick.h"
#include "Mario.h"
#include "PlayScene.h"
#include "Boomerang.h" 
CBoomerangBro::CBoomerangBro(float x, float y) :CGameObject(x, y)
{
    this->ax = 0;
    this->ay = BOOMERANG_BRO_GRAVITY;
    die_start = 0;
    walk_start = GetTickCount64();
    throw_start = 0;
    direction = -1;
    hasBoomerang = true;
    float boomerang_x = x + (direction > 0 ? 10.0f : -10.0f);
    float boomerang_y = y;
    boomerang = new CBoomerang(boomerang_x, boomerang_y, this);
    boomerang->SetActive(false);
    SetState(BOOMERANG_BRO_STATE_WALKING);
}

CBoomerangBro::~CBoomerangBro()
{
}

void CBoomerangBro::ThrowBoomerang()
{
    if (!hasBoomerang) return;

    // Calculate starting position relative to current BoomerangBro position
    float boomerang_x = x + (direction > 0 ? 10.0f : -10.0f);
    float boomerang_y = y;

    // Set the boomerang's position and initial position for its elliptical path
    boomerang->SetPosition(boomerang_x, boomerang_y);

    // Set direction
    boomerang->SetDirection(direction);

    // Activate and set state to the first movement state
    boomerang->SetActive(true);
    boomerang->SetState(BOOMERANG_STATE_1); // Use the first movement state

    hasBoomerang = false;
    DebugOut(L">>> BoomerangBro threw a boomerang! >>> \n");
}

void CBoomerangBro::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    if (state == BOOMERANG_BRO_STATE_DIE_TOP || state == BOOMERANG_BRO_STATE_DIE_BOTTOM)
    {
        left = x - BOOMERANG_BRO_BBOX_WIDTH / 2;
        top = y - BOOMERANG_BRO_BBOX_HEIGHT_DIE / 2;
        right = left + BOOMERANG_BRO_BBOX_WIDTH;
        bottom = top + BOOMERANG_BRO_BBOX_HEIGHT_DIE;
    }
    else
    {
        left = x - BOOMERANG_BRO_BBOX_WIDTH / 2;
        top = y - BOOMERANG_BRO_BBOX_HEIGHT / 2;
        right = left + BOOMERANG_BRO_BBOX_WIDTH;
        bottom = top + BOOMERANG_BRO_BBOX_HEIGHT;
    }
}

void CBoomerangBro::OnNoCollision(DWORD dt)
{
    x += vx * dt;
    y += vy * dt;
}

void CBoomerangBro::OnCollisionWith(LPCOLLISIONEVENT e)
{
    if (!e->obj->IsBlocking()) return;

    if (e->ny != 0)
    {
        vy = 0;
    }
    else if (e->nx != 0)
    {
        vx = -vx;
        direction = -direction;
    }
}

void CBoomerangBro::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    vy += ay * dt;
    vx += ax * dt;

    // Check if the BoomerangBro is dead
    if ((state == BOOMERANG_BRO_STATE_DIE_TOP || state == BOOMERANG_BRO_STATE_DIE_BOTTOM) && GetTickCount64() - die_start > BOOMERANG_BRO_DIE_TIMEOUT)
    {
        isDeleted = true;
        return;
    }

    CMario* mario = NULL;
    CPlayScene* scene = dynamic_cast<CPlayScene*>(CGame::GetInstance()->GetCurrentScene());
    if (scene)
    {
        mario = dynamic_cast<CMario*>(scene->GetPlayer());
    }

    // Update direction to face Mario
    if (mario)
    {
        float mario_x, mario_y;
        mario->GetPosition(mario_x, mario_y);

        if (mario_x > x)
            direction = 1;  // Mario is to the right, face right
        else
            direction = -1; // Mario is to the left, face left
    }

    // Check if boomerang has completed its path
    if (!hasBoomerang && boomerang && !boomerang->IsActive())
    {
        hasBoomerang = true;
        throw_cooldown = GetTickCount64();
    }

    // State transitions
    if (state == BOOMERANG_BRO_STATE_WALKING)
    {
        if (GetTickCount64() - walk_start > 2000)
        {
            vx = -vx;
            walk_start = GetTickCount64();
        }

        if (GetTickCount64() - walk_start > 1000 && hasBoomerang && GetTickCount64() - throw_cooldown > BOOMERANG_BRO_THROW_COOLDOWN)
        {
            SetState(BOOMERANG_BRO_STATE_IDLE);
        }
    }
    else if (state == BOOMERANG_BRO_STATE_IDLE && hasBoomerang && GetTickCount64() - throw_cooldown > BOOMERANG_BRO_THROW_COOLDOWN)
    {
        SetState(BOOMERANG_BRO_STATE_THROWING);
    }
    else if (state == BOOMERANG_BRO_STATE_THROWING && GetTickCount64() - throw_start > 500)
    {
        ThrowBoomerang();
        SetState(BOOMERANG_BRO_STATE_WALKING);
    }

}

void CBoomerangBro::Render()
{
    int aniId = -1;

    switch (state)
    {
    case BOOMERANG_BRO_STATE_IDLE:
        aniId = (direction > 0) ? ID_ANI_BOOMERANG_BRO_IDLE_RIGHT : ID_ANI_BOOMERANG_BRO_IDLE_LEFT;
        break;
    case BOOMERANG_BRO_STATE_WALKING:
        aniId = (direction > 0) ? ID_ANI_BOOMERANG_BRO_WALKING_RIGHT : ID_ANI_BOOMERANG_BRO_WALKING_LEFT;
        break;
    case BOOMERANG_BRO_STATE_THROWING:
        aniId = (direction > 0) ? ID_ANI_BOOMERANG_BRO_THROW_RIGHT : ID_ANI_BOOMERANG_BRO_THROW_LEFT;
        break;
    case BOOMERANG_BRO_STATE_DIE_TOP:
        aniId = ID_ANI_BOOMERANG_BRO_DIE_TOP;
        break;
    case BOOMERANG_BRO_STATE_DIE_BOTTOM:
        aniId = ID_ANI_BOOMERANG_BRO_DIE_BOTTOM;
        break;
    default:
        aniId = ID_ANI_BOOMERANG_BRO_WALKING_LEFT;
        break;
    }

    CAnimations::GetInstance()->Get(aniId)->Render(x, y);
    RenderBoundingBox();
}

void CBoomerangBro::SetState(int state)
{
    CGameObject::SetState(state);

    switch (state)
    {
    case BOOMERANG_BRO_STATE_WALKING:
        vx = direction * BOOMERANG_BRO_WALKING_SPEED;
        walk_start = GetTickCount64();
        break;
    case BOOMERANG_BRO_STATE_IDLE:
        vx = 0;
        break;
    case BOOMERANG_BRO_STATE_THROWING:
        vx = 0;
        throw_start = GetTickCount64();
        break;
    case BOOMERANG_BRO_STATE_DIE_TOP:
    case BOOMERANG_BRO_STATE_DIE_BOTTOM:
        die_start = GetTickCount64();
        vx = 0;
        vy = 0;
        ay = 0;
        break;
    }
}