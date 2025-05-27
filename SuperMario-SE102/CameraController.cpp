#include "CameraController.h"
#include "Mario.h"
#include "Collision.h"
#include "debug.h"
#include "Textures.h"
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

    pushSpeed = 0.01f;
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
    // In PUSH_FORWARD mode, create a narrow padding zone on the left edge
    if (mode == PUSH_FORWARD)
    {
        // Create a narrow strip (50 pixels wide) on the left side of the screen
        left = x;
        top = y;
        right = x;
        bottom = y + 500;
    }
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
        y = screenHeight / 2-80;
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
    case FREEZE_MODE:
        if (this->offSetX == 48)
        {
            x += this->offSetX;
            this->offSetX = 0;

            y += this->offSetY;
            this->offSetY = 0;
        }
        break;
    }

    game->SetCamPos(x, y);
}
// Add to CameraController.cpp
void CameraController::OnCollisionWith(LPCOLLISIONEVENT e)
{
}

void CameraController::OnOverlapWith(LPGAMEOBJECT obj)
{
    // Check if this is Mario
    CMario* mario = dynamic_cast<CMario*>(obj);
    if (!mario) return;

    // Get bounding boxes
    float camLeft, camTop, camRight, camBottom;
    this->GetBoundingBox(camLeft, camTop, camRight, camBottom);

    float marioLeft, marioTop, marioRight, marioBottom;
    mario->GetBoundingBox(marioLeft, marioTop, marioRight, marioBottom);

    if (marioLeft < camRight) {
        float pushDistance = camRight - marioLeft + 1.0f;  

        float marioX, marioY;
        mario->GetPosition(marioX, marioY);

        // Push Mario to the right
        mario->SetPosition(marioX + pushDistance, marioY);

        // Optionally prevent Mario from moving left 
        float vx, vy;
        mario->GetSpeed(vx, vy);
        if (vx < 0)
            mario->SetSpeed(0, vy);

        DebugOut(L"[CAMERA PUSH] Camera pushed Mario: %.2f units\n", pushDistance);
    }
}


void CameraController::RenderBoundingBox()
{
    if (mode != PUSH_FORWARD) return;

    float l, t, r, b;
    GetBoundingBox(l, t, r, b);

    RECT rect;
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)(r - l);
    rect.bottom = (int)(b - t);

    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);

    // ✅ Đây mới là cách đúng: dịch theo camera
    CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, BBOX_ALPHA);
}


void CameraController::SetFreeCameraDirection(int direction, bool enable)
{
    if (enable)
        freeCameraDirection |= direction;
    else
        freeCameraDirection &= ~direction;
}
