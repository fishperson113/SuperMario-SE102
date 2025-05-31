#pragma once
#include <d3dx9.h>
#include "GameObject.h"

enum CameraMode {
    THRESHOLD_BASED_TEST,
    PUSH_FORWARD_TEST,
    FREEZE_MODE_TEST
};

class Camera
{
private:
    static Camera* __instance;

    // Camera position
    float x, y;

    // Camera boundaries
    float leftBoundary;
    float rightBoundary;
    float topBoundary;
    float bottomBoundary;

    // Threshold values (for THRESHOLD_BASED mode)
    float leftThreshold;
    float rightThreshold;
    float topThreshold;
    float bottomThreshold;

    // Push speed (for PUSH_FORWARD mode)
    float pushSpeed;

    // Stop point (for PUSH_FORWARD mode)
    float stopPointX;
    bool hasStopPoint;

    // Screen dimensions
    int screenWidth;
    int screenHeight;

    // Current mode
    CameraMode mode;

    // Target object to follow (usually player)
    LPGAMEOBJECT target;

    // Private constructor for singleton
    Camera();

public:
    static Camera* GetInstance();

    void Update(DWORD dt);

    // Set camera target (usually the player)
    void SetTarget(LPGAMEOBJECT target) { this->target = target; }

    // Mode setters
    void SetMode(CameraMode newMode) { mode = newMode; }
    void SwitchToThresholdMode() { mode = THRESHOLD_BASED_TEST; }
    void SwitchToPushMode() { mode = PUSH_FORWARD_TEST; }
	void SwitchToFreezeMode() { mode = FREEZE_MODE_TEST; }
    // Getters and setters
    void SetPosition(float x, float y);
    void GetPosition(float& x, float& y) { x = this->x; y = this->y; }

    void SetBoundaries(float left, float right, float top, float bottom) {
        leftBoundary = left; rightBoundary = right;
        topBoundary = top; bottomBoundary = bottom;
    }

    void SetThresholds(float left, float right, float top, float bottom) {
        leftThreshold = left; rightThreshold = right;
        topThreshold = top; bottomThreshold = bottom;
    }

    void SetPushSpeed(float speed) { pushSpeed = speed; }
    void SetStopPoint(float x) { stopPointX = x; hasStopPoint = true; }

    CameraMode GetMode() const { return mode; }

    // Gets world position to camera space position
    D3DXVECTOR3 GetPositionInCamera(float worldX, float worldY);

    // Check if a world position is visible in camera view
    bool IsInView(float worldX, float worldY);
    void PushTarget();
    // Clean up
    void Unload();
    ~Camera();
};