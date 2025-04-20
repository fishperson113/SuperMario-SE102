#include "CBackgroundObject.h"

CBackgroundObject::CBackgroundObject(float x, float y, int spriteId) : CGameObject(x, y) {
    this->spriteId = spriteId;
}

void CBackgroundObject::Render() {
    LPSPRITE sprite = CSprites::GetInstance()->Get(spriteId);
    if (sprite != nullptr) {
        sprite->DrawOnWorld(x, y);
    }
}
