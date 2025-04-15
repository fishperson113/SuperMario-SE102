#include "Platform.h"
#include "Component.h"
#include "AssetIDs.h"

CPlatform::CPlatform(float x, float y, float cell_width, float cell_height, int length, int sprite_id_begin, int sprite_id_middle, int sprite_id_end)
{
	DebugOut(L"[INFO] Platform object has been created!\n");
	this->active = true;

	this->length = length;
	this->cellWidth = cell_width;
	this->cellHeight = cell_height;
	this->spriteIdBegin = sprite_id_begin;
	this->spriteIdMiddle = sprite_id_middle;
	this->spriteIdEnd = sprite_id_end;

	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(x, y);

	auto collider = AddComponent<ColliderComponent>();
	collider->SetBoundingBox(0, 0, this->cellWidth * (this->length + 1), this->cellHeight);
	collider->SetAsPlatform();

	auto animation = AddComponent<AnimationComponent>();
	LPANIMATION ani = new CAnimation(100);
	ani->CreateFrame(500);
	ani->AddSprite(this->spriteIdMiddle, 0, 0);
	for (int i = 1; i < length / 2; i++)
	{
		ani->AddSprite(this->spriteIdMiddle, this->cellWidth * i, 0);
		ani->AddSprite(this->spriteIdMiddle, - this->cellWidth * i, 0);
	}
	ani->AddSprite(this->spriteIdEnd, this->cellWidth * this->length / 2, 0);
	ani->AddSprite(this->spriteIdBegin, - this->cellWidth * this->length / 2, 0);
	animation->SetCurrentAnimation(ani);
	ani->Render(transform->GetPositionX(), transform->GetPositionY());
}

void CPlatform::Render()
{

}

