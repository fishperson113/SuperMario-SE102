#include "JumpingGoomba.h"
#include "AssetIDs.h"
#include "JumpingGoombaController.h"

CJumpingGoomba::CJumpingGoomba()
{
	AddComponent<CJumpingGoombaController>()->Awake();
}

CJumpingGoomba::~CJumpingGoomba()
{
}
