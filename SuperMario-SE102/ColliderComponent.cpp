#include "Component.h"
#include "GameObject.h"
#include "Textures.h"
#include "Game.h"
#include "debug.h"
void ColliderComponent::Render()
{
	if (!isActivatedRenderBoundingBox) return;
	auto transform = parentObject->GetComponent<TransformComponent>();
    D3DXVECTOR3 p(transform->GetPositionX(), transform->GetPositionY(), 0);
    RECT rect;

    LPTEXTURE bbox = CTextures::GetInstance()->Get(ID_TEX_BBOX);
    if (bbox == nullptr) {
        DebugOut(L"[ERROR] Bbox texture is null\n");
        return;
    }

    float l, t, r, b;

    GetBoundingBox(l, t, r, b);
    rect.left = 0;
    rect.top = 0;
    rect.right = (int)r - (int)l;
    rect.bottom = (int)b - (int)t;

    float cx, cy;
    CGame::GetInstance()->GetCamPos(cx, cy);

    float x = transform->GetPositionX();
	float y = transform->GetPositionY();

    CGame::GetInstance()->Draw(x - cx, y - cy, bbox, &rect, 0.25);
}

void ColliderComponent::SetBoundingBox(float left, float top, float width, float height)
{
    this->left = left;
    this->top = top;
    this->width = width;
    this->height = height;
}

void ColliderComponent::GetBoundingBox(float& left, float& top, float& right, float& bottom) const
{
    auto transform = parentObject->GetComponent<TransformComponent>();
    if (transform)
    {
		//bounding box is relative to the object's position
        left = transform->GetPositionX() + this->left;
        top = transform->GetPositionY() + this->top;
        right = left + width;
        bottom = top + height;
    }
    else
    {
        // By default
        left = this->left;
        top = this->top;
        right = left + width;
        bottom = top + height;
    }
}

int ColliderComponent::IsDirectionColliable(float nx, float ny) const
{

    if (!isCollidable) return 0;

    if (nx == 0 && ny == -1) {
        return collidableFromTop ? 1 : 0;
    }
    else if (nx == 0 && ny == 1) {
        return collidableFromBottom ? 1 : 0;
    }
    else if (nx == -1 && ny == 0) {
        return collidableFromLeft ? 1 : 0;
    }
    else if (nx == 1 && ny == 0) {
        return collidableFromRight ? 1 : 0;
    }

    return 1;
}