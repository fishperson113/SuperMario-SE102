#include "CameraTest.h"
#include "Game.h"
#include "PlayScene.h"
#include "debug.h"

Camera* Camera::__instance = NULL;

Camera::Camera()
{
    x = y = 0;
    leftBoundary = -10;
    rightBoundary = 10000;
    topBoundary = -10000;
    bottomBoundary = 10000;

    // No longer needed for direct following
    leftThreshold = 0.3f;
    rightThreshold = 0.3f;
    topThreshold = 0.3f;
    bottomThreshold = 0.3f;

    pushSpeed = 0.03f;
    stopPointX = 0;
    hasStopPoint = false;

    // Get screen dimensions from game
    CGame* game = CGame::GetInstance();
    screenWidth = game->GetBackBufferWidth();
    screenHeight = game->GetBackBufferHeight();

    // Default mode
    mode = THRESHOLD_BASED_TEST;
    target = NULL;
}

Camera* Camera::GetInstance()
{
    if (__instance == NULL) __instance = new Camera();
    return __instance;
}

void Camera::SetPosition(float x, float y)
{
    this->x = x;
    this->y = y;

    CGame::GetInstance()->SetCamPos(x, y);
}

D3DXVECTOR3 Camera::GetPositionInCamera(float worldX, float worldY)
{
    return D3DXVECTOR3(worldX - x, worldY - y, 0);
}

bool Camera::IsInView(float worldX, float worldY)
{
    bool xInView = worldX >= x && worldX < x + screenWidth;
    bool yInView = worldY >= y && worldY < y + screenHeight;
    return xInView && yInView;
}

void Camera::PushTarget()
{
    if (!target || mode != PUSH_FORWARD_TEST) return;

    CMario* mario = dynamic_cast<CMario*>(target);
    if (!mario) return;

    float marioX, marioY;
    mario->GetPosition(marioX, marioY);

    float leftEdge = x + 16.0f; 

    if (marioX < leftEdge)
    {

        mario->SetPosition(leftEdge, marioY);

        float vx, vy;
        mario->GetSpeed(vx, vy);
        if (vx < 0) // If Mario is moving left
            mario->SetSpeed(0, vy); // Stop horizontal movement

        DebugOut(L"[CAMERA] Pushed Mario right: %.2f\n", leftEdge);
    }
}

void Camera::Update(DWORD dt)
{
    CGame* game = CGame::GetInstance();

    // Get current scene
    CPlayScene* currentScene = dynamic_cast<CPlayScene*>(game->GetCurrentScene());
    bool isPaused = currentScene && currentScene->IsPaused();
    bool isGameOver = currentScene && currentScene->IsGameOver();

    // Don't update camera if game is paused or over
    if (isPaused || isGameOver) {
        game->SetCamPos(x, y);
        return;
    }

    // Update screen dimensions in case they changed
    screenWidth = game->GetBackBufferWidth();
    screenHeight = game->GetBackBufferHeight();

    // Don't update if no target is set
    if (target == NULL) {
        game->SetCamPos(x, y);
        return;
    }

    // Get target position
    float targetX = 0, targetY = 0;
    if (target) {
        target->GetPosition(targetX, targetY);
    }
    CMario* mario = dynamic_cast<CMario*>(target);

    // Update camera position based on mode
    switch (mode)
    {
    case THRESHOLD_BASED_TEST:
    {
        x = targetX - screenWidth / 2;

        float absoluteTopThreshold = y + (topThreshold * screenHeight);
        float absoluteBottomThreshold = y + ((1.0f - bottomThreshold) * screenHeight);

        if (targetY > absoluteBottomThreshold)
        {
            float moveAmount = (targetY - absoluteBottomThreshold);
            y += moveAmount;
        }
        else if (targetY < absoluteTopThreshold)
        {
            float moveAmount = (absoluteTopThreshold - targetY);
            y -= moveAmount;
        }
        break;
    }
    case PUSH_FORWARD_TEST:
        if (hasStopPoint && x >= stopPointX) {
            DebugOut(L"[CAMERA] Reached stop point at x = %.2f. Switching to threshold mode.\n", stopPointX);
            SwitchToThresholdMode();
        }
        else {
            x += pushSpeed * dt;
            y = targetY - screenHeight / 2;
            PushTarget();
        }
        break;

    case FREEZE_MODE_TEST:
        x = 2054;  // Fixed X position
        y = 200;   // Fixed Y position
        if (mario && targetY <= 134 && mario->GetIsUnderground()) {
            // Switch back to threshold mode
            mode = THRESHOLD_BASED_TEST;
            DebugOut(L"[CAMERA] Mario returned above ground. Switching to THRESHOLD_MODE.\n");
        }
        break;
    }

    // Apply boundaries
    x = max(leftBoundary, min(x, rightBoundary - screenWidth));
    y = max(topBoundary, min(y, bottomBoundary - screenHeight));

    if (y > bottomBoundary - screenHeight)
        y = bottomBoundary - screenHeight;

    game->SetCamPos(x, y);
}

void Camera::Unload()
{
    if (__instance != NULL) {
        delete __instance;
        __instance = NULL;
    }
}

Camera::~Camera()
{
    // Nothing to clean up
}