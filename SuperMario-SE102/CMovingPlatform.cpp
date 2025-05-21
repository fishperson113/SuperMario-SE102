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

    // Set vx, vy based on direction
    switch (moveDirection)
    {
    case PLATFORM_MOVE_LEFT:
        vx = -moveSpeed;
        vy = 0;
        break;
    case PLATFORM_MOVE_RIGHT:
        vx = moveSpeed;
        vy = 0;
        break;
    case PLATFORM_MOVE_UP:
        vx = 0;
        vy = -moveSpeed;
        break;
    case PLATFORM_MOVE_DOWN:
        vx = 0;
        vy = moveSpeed;
        break;
    default:
        vx = 0;
        vy = 0;
        break;
    }

    // Predict new position
    float newX = x + vx * dt;
    float newY = y + vy * dt;

    // Horizontal boundaries
    if (vx < 0 && newX < leftLimit) {
        x = leftLimit;
        moveDirection = PLATFORM_MOVE_RIGHT;
        vx = moveSpeed;
    }
    else if (vx > 0 && newX > rightLimit) {
        x = rightLimit;
        moveDirection = PLATFORM_MOVE_LEFT;
        vx = -moveSpeed;
    }
    else {
        x = newX;
    }

    // Vertical boundaries
    if (vy < 0 && newY < topLimit) {
        y = topLimit;
        moveDirection = PLATFORM_MOVE_DOWN;
        vy = moveSpeed;
    }
    else if (vy > 0 && newY > bottomLimit) {
        y = bottomLimit;
        moveDirection = PLATFORM_MOVE_UP;
        vy = -moveSpeed;
    }
    else {
        y = newY;
    }
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
	return vx;
}

float CMovingPlatform::GetSpeedY()
{
    return vy;
}


void CMovingPlatform::OnCollisionWith(LPCOLLISIONEVENT e)
{
   
}

void CMovingPlatform::OnOverlapWith(LPGAMEOBJECT obj)
{

}
