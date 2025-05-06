#include "Checkpoint.h"
#include"ObjectManager.h"
#include "debug.h"
Checkpoint::Checkpoint(float x, float y, float width, float height,ObjectManager* objectManager) : CGameObject(x, y)
{
	this->width = width;
	this->height = height;
	isActivated = false;
	state = CHECKPOINT_STATE_INACTIVE;
	this->objectManager = objectManager;
}

void Checkpoint::AddObjectToSpawn(LPGAMEOBJECT obj)
{
	if (obj) objectsToSpawn.push_back(obj);
}

void Checkpoint::AddObjectsToSpawn(const std::vector<LPGAMEOBJECT>& objects)
{
	for (auto obj : objects)
		if (obj) objectsToSpawn.push_back(obj);
}

void Checkpoint::SpawnObjects()
{
	if (objectsToSpawn.empty() || objectManager == nullptr)
		return;

	DebugOut(L"[INFO] Checkpoint triggered! Spawning %d objects\n", objectsToSpawn.size());

	for (auto obj : objectsToSpawn)
	{
		if (obj && !obj->IsDeleted())
		{
			objectManager->Add(obj);
		}
	}
	objectsToSpawn.clear();
}

void Checkpoint::Render()
{
	RenderBoundingBox();
}

void Checkpoint::Update(DWORD dt)
{
}

void Checkpoint::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x - width / 2;
	top = y - height / 2;
	right = left + width;
	bottom = top + height;
}

void Checkpoint::SetState(int state)
{
	CGameObject::SetState(state);

	if (state == CHECKPOINT_STATE_ACTIVE)
		SpawnObjects();
}
