#include "CMovingPlatform.h"
#include "Mario.h"
#include "debug.h"
#include "Game.h"

CMovingPlatform::CMovingPlatform(
    float x, float y,
    float cell_width, float cell_height, int length,
    int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
    float move_speed) :
    CPlatform(x, y, cell_width, cell_height, length, sprite_id_begin, sprite_id_middle, sprite_id_end)
{
    this->moveSpeed = move_speed;
    this->moveDirection = PLATFORM_MOVE_RIGHT; // Always start moving right
    this->hasMarioTouched = false; // Initialize to false
}

void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // Set velocity based on current direction
    if (moveDirection == PLATFORM_MOVE_RIGHT) {
        vx = moveSpeed;
        vy = 0;
    }
    else if (moveDirection == PLATFORM_MOVE_DOWN) {
        vx = 0;
        vy = moveSpeed;
    }

    // Update position
    x += vx * dt;
    y += vy * dt;
}

void CMovingPlatform::Render()
{
    CPlatform::Render();
}

float CMovingPlatform::GetSpeedX()
{
    return vx;
}

float CMovingPlatform::GetSpeedY()
{
    return vy;
}

void CMovingPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
}
