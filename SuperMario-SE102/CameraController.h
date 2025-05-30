#pragma once
#include "Game.h"
#include "GameObject.h"

class CMario;
enum CameraMoveMode {
    FOLLOW_PLAYER,     
    PUSH_FORWARD,      
    THRESHOLD_BASED,
	FREE_MOVE,
    FREEZE_MODE
};
class CameraController : public CGameObject {
private:

    CameraMoveMode mode;

    float x, y;
    float stopPointX; 
    bool hasStopPoint;

    // Camera boundaries
    float leftBoundary;
    float rightBoundary;
    float topBoundary;     
    float bottomBoundary;

    float leftThreshold;    
    float rightThreshold;   
	float topThreshold;
	float bottomThreshold;
    // Push speed when in PUSH_FORWARD mode
    float pushSpeed;
	// Free movement speed
    float freeMovementSpeed; 
    int freeCameraDirection;

    int screenWidth;
    int screenHeight;

    float offSetX = 48;
	float offSetY = 16;
    // Reference to the game and player
    CGame* game;
    LPGAMEOBJECT player;

public:
    CameraController(LPGAMEOBJECT player, CGame* game);
    virtual ~CameraController() = default;

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render() { RenderBoundingBox(); }  // Empty render - camera doesn't need to be drawn
    virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

    void SetMode(CameraMoveMode newMode) { mode = newMode; }

    void SetBoundaries(float left, float right, float top, float bottom) {
        leftBoundary = left; rightBoundary = right;
        topBoundary = top; bottomBoundary = bottom;
    }

    void SetThresholds(float left, float right) {
        leftThreshold = left; rightThreshold = right;
    }

    void SetVerticalThresholds(float top, float bottom) {
        topThreshold = top; bottomThreshold = bottom;
    }
    void RenderBoundingBox() override;

    void SetPushSpeed(float speed) { pushSpeed = speed; }
    void SetFreeMovementSpeed(float speed) { freeMovementSpeed = speed; }

    void SetFreeCameraDirection(int direction, bool enable);

    // Override GameObject methods to make camera a special object
	virtual int IsDynamic() { return 1; } 
    virtual void OnCollisionWith(LPCOLLISIONEVENT e);
    virtual void OnOverlapWith(LPGAMEOBJECT obj) override;

    // Debug functions to switch modes
    void SwitchToFollowMode() { mode = FOLLOW_PLAYER; }
    void SwitchToPushMode() { mode = PUSH_FORWARD; }
    void SwitchToThresholdMode() { mode = THRESHOLD_BASED; }
    void SwitchToFreeMove() { mode = FREE_MOVE; }
	void SwitchToFreezeMode() { mode = FREEZE_MODE; }

    bool IsInFreeMove() const { return mode == FREE_MOVE; }
    void SetStopPoint(float x) { stopPointX = x; hasStopPoint = true; }
    int GetMode() const { return mode; }
};
enum FreeCameraDirection {
    FREE_CAM_NONE = 0,
    FREE_CAM_LEFT = 1,
    FREE_CAM_RIGHT = 2,
    FREE_CAM_UP = 4,
    FREE_CAM_DOWN = 8
};


