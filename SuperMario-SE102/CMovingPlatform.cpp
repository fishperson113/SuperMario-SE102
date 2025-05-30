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
    this->moveDirection = PLATFORM_MOVE_LEFT; // Always start moving right
    this->hasMarioTouched = false; // Initialize to false
	this->isInView = false; // Initialize to false
}

void CMovingPlatform::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // Check if platform is in camera view
    bool nowInView = IsInCameraView();

    // If platform has just come into view, mark it as visible
    if (nowInView && !isInView) {
        isInView = true;
        DebugOut(L"[INFO] Moving platform entered camera view at position (%f, %f)\n", x, y);
    }
    if(!isInView) {
        vx = 0;
        vy = 0;
        return;
	}
    // Set velocity based on current direction
    if (moveDirection == PLATFORM_MOVE_RIGHT) {
        vx = moveSpeed;
        vy = 0;
    }
    else if (moveDirection == PLATFORM_MOVE_DOWN) {
        vx = 0;
        vy = moveSpeed;
    }
    else if (moveDirection == PLATFORM_MOVE_LEFT) {
        vx = -moveSpeed;
        vy = 0;
    }
    else if (moveDirection == PLATFORM_MOVE_UP) {
        vx = 0;
        vy = -moveSpeed;
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

bool CMovingPlatform::IsInCameraView()
{
    CGame* game = CGame::GetInstance();
    float camX, camY;
    game->GetCamPos(camX, camY);

    // Get screen dimensions
    int screenWidth = game->GetBackBufferWidth();
    int screenHeight = game->GetBackBufferHeight();

    // Get platform's bounding box
    float left, top, right, bottom;
    GetBoundingBox(left, top, right, bottom);

    // Simple check if the platform is within the camera's view
    return (right >= camX && left <= camX + screenWidth &&
        bottom >= camY && top <= camY + screenHeight);

}
