#pragma once
#include "GameObject.h"

enum class PipeDirection
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

class CPipe : public CGameObject
{
protected:
	int length;				// Unit: cell 
	float cellWidth;
	float cellHeight;
	int spriteIdBegin, spriteIdMiddle, spriteIdEnd;

	PipeDirection entryDirection;    // Direction Mario enters this pipe
	PipeDirection exitDirection;    // Direction Mario enters/exits
	bool isEntrance;            // Is this an entrance pipe
	bool isExit;                // Is this an exit pipe
	float targetX, targetY;     // Target coordinates when Mario enters

public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;

		this->entryDirection = PipeDirection::DOWN;
		this->exitDirection = PipeDirection::UP;
		this->isEntrance = false;
		this->isExit = false;
		this->targetX = 0.0f;
		this->targetY = 0.0f;
	}

	void Render();
	void Update(DWORD dt) {}
	void GetBoundingBox(float& l, float& t, float& r, float& b);
	void RenderBoundingBox();

	int IsDirectionColliable(float nx, float ny);

	// New methods for teleportation
	void SetAsEntrance(float targetX, float targetY, PipeDirection entryDir, PipeDirection exitDir);
	void SetAsExit();
	bool IsEntrance() const { return isEntrance; }
	bool IsExit() const { return isExit; }
	void GetTargetPosition(float& x, float& y) const;
	PipeDirection GetEntryDirection() const { return entryDirection; }
	PipeDirection GetExitDirection() const { return exitDirection; }
	void GetEntryPosition(float& x, float& y) const;
	void GetExitPosition(float& x, float& y) const;
};

typedef CPipe* LPPIPE;