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
    this->moveDirection = PLATFORM_MOVE_RIGHT; // Default direction
    this->isAutoMoving = true; // Auto move by default

    this->leftLimit = x - 100.0f;
    this->rightLimit = x + 100.0f;
    this->topLimit = y - 100.0f;
    this->bottomLimit = y + 100.0f;
}


void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (!isAutoMoving)
        return; 

    float dx = 0, dy = 0;

    switch (moveDirection)
    {
    case PLATFORM_MOVE_LEFT:
        dx = -moveSpeed * dt;
        break;
    case PLATFORM_MOVE_RIGHT:
        dx = moveSpeed * dt;
        break;
    case PLATFORM_MOVE_UP:
        dy = -moveSpeed * dt;
        break;
    case PLATFORM_MOVE_DOWN:
        dy = moveSpeed * dt;
        break;
    }

    if (dx < 0 && x + dx < leftLimit) {
        x = leftLimit;
        moveDirection = PLATFORM_MOVE_RIGHT;  // Change direction
        dx = 0;
    }
    else if (dx > 0 && x + dx > rightLimit) {
        // Reached right boundary
        x = rightLimit;
        moveDirection = PLATFORM_MOVE_LEFT;  // Change direction
        dx = 0;
    }

    // Handle vertical movement boundaries
    if (dy < 0 && y + dy < topLimit) {
        // Reached top boundary
        y = topLimit;
        moveDirection = PLATFORM_MOVE_DOWN;  // Change direction
        dy = 0;
    }
    else if (dy > 0 && y + dy > bottomLimit) {
        // Reached bottom boundary
        y = bottomLimit;
        moveDirection = PLATFORM_MOVE_UP;  // Change direction
        dy = 0;
    }

    // Apply movement
    x += dx;
    y += dy;

    vx = dx / dt;
    vy = dy / dt;
}


void CMovingPlatform::Render()
{
    CPlatform::Render(); 
}

void CMovingPlatform::SetLimits(float left, float right, float top, float bottom)
{
    this->leftLimit = left;
    this->rightLimit = right;
    this->topLimit = top;
    this->bottomLimit = bottom;
}

void CMovingPlatform::SetMoveDirection(int direction)
{
    this->moveDirection = direction;
}

float CMovingPlatform::GetSpeedX()
{
	return moveSpeed * (moveDirection == PLATFORM_MOVE_LEFT ? -1 : 1);
}

float CMovingPlatform::GetSpeedY()
{
    return moveSpeed*(moveDirection== PLATFORM_MOVE_UP?-1:1);
}


void CMovingPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
   
}

void CMovingPlatform::OnOverlapWith(LPGAMEOBJECT obj)
{

}
