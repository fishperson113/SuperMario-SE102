#pragma once
#include "GameObject.h"

class CEffect : public CGameObject
{
protected:
    ULONGLONG start_time;
    int lifetime;  // Duration in milliseconds before the effect is removed

public:
    CEffect(float x, float y, int lifetime = 1000);
    virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
    virtual void Render() = 0;
    virtual void GetBoundingBox(float& l, float& t, float& r, float& b);

    int IsBlocking() { return 0; }
    int IsCollidable() { return 0; }
};