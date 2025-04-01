#pragma once

#include "Sprite.h"

struct SpriteInfo {
    LPSPRITE sprite;
    float offsetX;
    float offsetY;

    SpriteInfo(LPSPRITE s, float ox, float oy) :
        sprite(s), offsetX(ox), offsetY(oy) {
    }
};

class CAnimationFrame
{
    std::vector<SpriteInfo> sprites;
    DWORD time;

public:
    CAnimationFrame(int time) {
        this->time = time;
    }

    void AddSprite(LPSPRITE sprite, float offsetX = 0, float offsetY = 0) {
        sprites.push_back(SpriteInfo(sprite, offsetX, offsetY));
    }

    DWORD GetTime() { return time; }

    void Draw(float x, float y) {
        for (const SpriteInfo& info : sprites) {
            if (info.sprite) {
                info.sprite->Draw(x + info.offsetX, y + info.offsetY);
            }
        }
    }
};

typedef CAnimationFrame* LPANIMATION_FRAME;