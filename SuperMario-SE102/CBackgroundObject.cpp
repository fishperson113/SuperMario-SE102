#include "CBackgroundObject.h"

CBackgroundObject::CBackgroundObject(float x, float y, int spriteId, int objectCount, float offsetX, float offsetY)
    : CGameObject(x, y)
{
    this->spriteId = spriteId;
    this->objectCount = objectCount;
    this->offsetX = offsetX;
    this->offsetY = offsetY;
}

void CBackgroundObject::Render() {
    LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
    if (sprite != nullptr) {
        for (int i = 0; i < objectCount; i++)
        {
            float posX = x + i * offsetX;
            float posY = y + i * offsetY;
            sprite->Draw(posX, posY);
        }
    }
}
