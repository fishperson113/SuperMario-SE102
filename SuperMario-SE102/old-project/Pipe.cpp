#include "Pipe.h"
#include "debug.h"
#include "Component.h"
#include "AssetIDs.h"

CPipe::CPipe(float x, float y, float cell_width, float cell_height, int length, int sprite_id_begin, int sprite_id_middle, int sprite_id_end)
{
	DebugOut(L"[INFO] Pipe object has been created!\n");
	this->active = true;

	this->length = length;
	this->cellWidth = cell_width;
	this->cellHeight = cell_height;
	this->spriteIdBegin = sprite_id_begin;
	this->spriteIdMiddle = sprite_id_middle;
	this->spriteIdEnd = sprite_id_end;

	auto animation = AddComponent<AnimationComponent>();

	auto transform = AddComponent<TransformComponent>();
	transform->SetPosition(x, y);

	AddComponent<AnimationComponent>();
	auto collider = AddComponent<ColliderComponent>();
	collider->SetCollidableDirections(true, false, false, false);

	collider->SetBoundingBox(0, 0, this->cellWidth, this->cellHeight * this->length);

	LPANIMATION ani = new CAnimation(100);
	ani->CreateFrame(500);
	ani->AddSprite(52001, 0, 0);
	for (int i = 1; i < this->length; i++)
		ani->AddSprite(51001, 0, this->cellHeight * i);
	animation->SetCurrentAnimation(ani);
	ani->Render(transform->GetPositionX(), transform->GetPositionY());
}

void CPipe::Render()
{
}
