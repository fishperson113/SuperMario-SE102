#include "CEffect.h"
#include "PlayScene.h"
#include "debug.h"

CEffect::CEffect(float x, float y, int lifetime) : CGameObject(x, y)
{
    this->lifetime = lifetime;
    this->start_time = GetTickCount64();
}

void CEffect::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
    if (GetTickCount64() - start_time > lifetime)
    {
        this->Delete();
        return;
    }

    x += vx * dt;
    y += vy * dt;
}

void CEffect::GetBoundingBox(float& l, float& t, float& r, float& b)
{
    l = t = r = b = 0;
}