#pragma once
#include "GameObject.h"
#define PIPE_TYPE_STANDARD         0  // Standard pipe
#define PIPE_TYPE_SHORT            1  // Short pipe for moving up
#define PIPE_TYPE_LONG             2  // Long pipe for moving down
#define PIPE_TYPE_WITH_FLOWER      3  // Pipe with flower enemy
#define PIPE_TYPE_WITH_STRIPES     4  // Pipe with striped decoration

#define DOWN_PIPE_POSITION_X       135
#define DOWN_PIPE_POSITION_Y       412
#define UP_PIPE_POSITION_X         2304
#define UP_PIPE_POSITION_Y         310
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
	int pipeType;

	PipeDirection entryDirection; 
	PipeDirection exitDirection;  
	bool isEntrance;           
	bool isExit;               
	float targetX, targetY;    

public:
	CPipe(float x, float y,
		float cell_width, float cell_height, int length,
		int sprite_id_begin, int sprite_id_middle, int sprite_id_end,
		int pipe_type = PIPE_TYPE_STANDARD) :CGameObject(x, y)
	{
		this->length = length;
		this->cellWidth = cell_width;
		this->cellHeight = cell_height;
		this->spriteIdBegin = sprite_id_begin;
		this->spriteIdMiddle = sprite_id_middle;
		this->spriteIdEnd = sprite_id_end;
		this->pipeType = pipe_type;

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
	int GetPipeType() const { return pipeType; }
	void SetPipeType(int type) { pipeType = type; }
};

typedef CPipe* LPPIPE;