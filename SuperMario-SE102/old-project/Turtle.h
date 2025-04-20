#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"
#include "TurtleController.h"

#define TURTLE_WIDTH 10
#define TURTLE_BBOX_WIDTH 10
#define TURTLE_BBOX_HEIGHT 16

class CTurtle : public CGameObject
{
public:
	CTurtle();
	~CTurtle();
};

