#include "CameraController.h"
#include "Mario.h"
#include "Collision.h"
#include "debug.h"

CameraController::CameraController(LPGAMEOBJECT player, CGame* game) : CGameObject()
{
    this->player = player;
    this->game = game;
    this->mode = player != NULL ? THRESHOLD_BASED : FREE_MOVE;

    leftBoundary = -10000;
    rightBoundary = 10000;
    topBoundary = -1000;
    bottomBoundary = 1000;

    leftThreshold = 0.3f;
    rightThreshold = 0.3f;
    topThreshold = 0.3f;
    bottomThreshold = 0.7f;

    pushSpeed = 0.05f;
    freeMovementSpeed = 0.2f;
    freeCameraDirection = 0;

    // Get initial screen dimensions
    screenWidth = game->GetBackBufferWidth();
    screenHeight = game->GetBackBufferHeight();

    // Set initial position
    game->GetCamPos(x, y);
}

void CameraController::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
    // The camera's bounding box is the entire screen
    left = x;
    top = y;
    right = x + screenWidth;
    bottom = y + screenHeight;
}

void CameraController::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    // Update screen dimensions in case they changed
    screenWidth = game->GetBackBufferWidth();
    screenHeight = game->GetBackBufferHeight();

    if (mode == FREE_MOVE || player == NULL)
    {
        // Handle free camera movement based on direction flags
        if (freeCameraDirection & FREE_CAM_LEFT)
            x -= freeMovementSpeed * dt;
        if (freeCameraDirection & FREE_CAM_RIGHT)
            x += freeMovementSpeed * dt;
        if (freeCameraDirection & FREE_CAM_UP)
            y -= freeMovementSpeed * dt;
        if (freeCameraDirection & FREE_CAM_DOWN)
            y += freeMovementSpeed * dt;

        // Apply boundaries
        x = max(leftBoundary, min(x, rightBoundary - screenWidth));
        y = max(topBoundary, min(y, bottomBoundary - screenHeight));

        game->SetCamPos(x, y);
        return;
    }

    // Get player position
    float playerX, playerY;
    player->GetPosition(playerX, playerY);

    // If the thresholds represent percentage from the respective edges
    float absoluteLeftThreshold = x + leftThreshold * screenWidth;
    float absoluteRightThreshold = x + rightThreshold * screenWidth;  // Changed calculation

    float absoluteTopThreshold = y + topThreshold * screenHeight;
    float absoluteBottomThreshold = y + bottomThreshold * screenHeight;  // Changed calculation


    // Update camera position based on mode
    switch (mode)
    {
    case FOLLOW_PLAYER:
        // Center the camera on the player
        x = playerX - screenWidth / 2;
        y = playerY - screenHeight / 2;
        break;

    case PUSH_FORWARD:
        // Move camera forward constantly
        x += pushSpeed * dt;

        break;

    case THRESHOLD_BASED:
        // Handle horizontal camera movement
        if (playerX > absoluteRightThreshold)
        {
            // Player is beyond right threshold, move camera
            x += (playerX - absoluteRightThreshold);
        }
        else if (playerX < absoluteLeftThreshold)
        {
            // Player is beyond left threshold, move camera 
            x = max(leftBoundary, x - (absoluteLeftThreshold - playerX));
        }

        // Handle vertical camera movement
        if (playerY > absoluteBottomThreshold)
        {
            // Player is beyond bottom threshold, move camera down
            y += (playerY - absoluteBottomThreshold);
        }
        else if (playerY < absoluteTopThreshold)
        {
            // Player is beyond top threshold, move camera up
            y = max(topBoundary, y - (absoluteTopThreshold - playerY));
        }
        break;
    }

    // Apply boundaries
    x = max(leftBoundary, min(x, rightBoundary - screenWidth));
    y = max(topBoundary, min(y, bottomBoundary - screenHeight));

    // Only in PUSH_FORWARD mode, check for collision with player
    if (mode == PUSH_FORWARD && player != NULL)
    {
        CMario* mario = dynamic_cast<CMario*>(player);
        if (mario)
        {
            OnCollisionWith(mario);
        }
    }

    // Update the actual camera position in the game engine
    game->SetCamPos(x, y);
}

void CameraController::OnCollisionWith(CMario* mario)
{
    float marioX, marioY;
    mario->GetPosition(marioX, marioY);

    float marioLeft, marioTop, marioRight, marioBottom;
    mario->GetBoundingBox(marioLeft, marioTop, marioRight, marioBottom);

    float cameraLeft, cameraTop, cameraRight, cameraBottom;
    GetBoundingBox(cameraLeft, cameraTop, cameraRight, cameraBottom);

    // Check if Mario is inside the camera view
    // If Mario's right edge is beyond the left edge of camera (plus buffer)
    if (marioRight < cameraLeft + 50)
    {
        // Push Mario to the right
        mario->SetPosition(cameraLeft + 50 + (marioRight - marioLeft) / 2, marioY);

        // Optionally stop horizontal movement
        float vx, vy;
        mario->GetSpeed(vx, vy);
        mario->SetSpeed(0.0f, vy);

        DebugOut(L">>> Camera pushed Mario to the right! >>> \n");
    }
}

void CameraController::SetFreeCameraDirection(int direction, bool enable)
{
    if (enable)
        freeCameraDirection |= direction;
    else
        freeCameraDirection &= ~direction;
}
