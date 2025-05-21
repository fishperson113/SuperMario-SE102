#pragma once
#include "Platform.h"

#define PLATFORM_MOVE_NONE  0
#define PLATFORM_MOVE_LEFT  1
#define PLATFORM_MOVE_RIGHT 2
#define PLATFORM_MOVE_UP    4
#define PLATFORM_MOVE_DOWN  8

class CMovingPlatform : public CPlatform
{
protected:
    float moveSpeed;        
    int moveDirection;      
    float leftLimit;        
    float rightLimit;       
    float topLimit;         
    float bottomLimit;      
    bool isAutoMoving;      

public:
    CMovingPlatform(
        float x, float y,
        float cell_width, float cell_height, int length,
        int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
        float move_speed = 0.05f);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();

    // Set boundary limits
    void SetLimits(float left, float right, float top, float bottom);

    // Set movement direction
    void SetMoveDirection(int direction);

    // Enable/disable auto movement
    void SetAutoMoving(bool isAuto) { isAutoMoving = isAuto; }

    // Implement collision interfaces
    int IsCollidable() { return 0; }
    int IsBlocking() { return 1; }
	int IsDynamic() { return 1; } 
    float GetSpeedX();
    float GetSpeedY();
	void OnCollisionWith(LPCOLLISIONEVENT e);
    // Handle overlap with other objects (especially Mario)
    virtual void OnOverlapWith(LPGAMEOBJECT obj);
};
