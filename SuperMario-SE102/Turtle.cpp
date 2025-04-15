#include "Turtle.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CTurtle::CTurtle()
{
	AddComponent<TurtleController>()->Awake();
}

CTurtle::~CTurtle()
{
}

