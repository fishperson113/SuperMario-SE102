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
    bool hasMarioTouched; 

public:
    CMovingPlatform(
        float x, float y,
        float cell_width, float cell_height, int length,
        int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
        float move_speed = 0.05f);

    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
    virtual void Render();

	void SetMarioTouched(bool touched) { hasMarioTouched = touched; }
	void SetMoveDirection(int direction) { moveDirection = direction; }
    int IsCollidable() { return 0; }
    int IsBlocking() { return 1; }
	int IsDynamic() { return 1; } 
    float GetSpeedX();
    float GetSpeedY();
	void OnCollisionWith(LPCOLLISIONEVENT e);
	int GetMoveDirection() { return moveDirection; }
};
