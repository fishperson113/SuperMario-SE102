#include "Goomba.h"
#include "AssetIDs.h"

CGoomba::CGoomba()
{
	AddComponent<GoombaController>()->Awake();
}

CGoomba::~CGoomba()
{
}


