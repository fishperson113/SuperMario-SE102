#include "Checkpoint.h"
#include"ObjectManager.h"
#include "debug.h"
#include "Game.h"
Checkpoint::Checkpoint(float x, float y, float width, float height) : CGameObject(x, y)
{
	this->width = width;
	this->height = height;
	isActivated = false;
	state = CHECKPOINT_STATE_INACTIVE;
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
	if (objectsToSpawn.empty())
		return;

	DebugOut(L"[INFO] Checkpoint triggered! Spawning %d objects\n", objectsToSpawn.size());

	for (auto obj : objectsToSpawn)
	{
		if (obj && !obj->IsDeleted())
		{
			obj->SetActive(true);
		}
	}
	objectsToSpawn.clear();
}

void Checkpoint::Render()
{
	//RenderBoundingBox();
}

void Checkpoint::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGame* game = CGame::GetInstance();
	if (game->IsKeyDown(DIK_C))
	{
		if (!isActivated)
		{
			DebugOut(L"[INFO] Manually triggering checkpoint at position (%.2f, %.2f)\n", x, y);
			SetState(CHECKPOINT_STATE_ACTIVE);
		}
	}
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
