#include "Mario.h"
#include "AssetIDs.h"

CMario::CMario()
{
	AddComponent<MarioController>()->Awake();
}

CMario::~CMario()
{
}

