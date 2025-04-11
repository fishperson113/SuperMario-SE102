#include "Turtle.h"
#include "debug.h"
#include "Game.h"
#include "Component.h"

CTurtle::CTurtle()
{
	AddComponent<TurtleController>();
}

CTurtle::~CTurtle()
{
}

void CTurtle::Update(DWORD dt)
{
	if (this->active == false) return;

	CGameObject::Update(dt);
	if (turtleController) {
		turtleController->Update(dt);
	}
}

void CTurtle::Render()
{
	CGameObject::Render();
}

void CTurtle::Awake()
{
	auto turtleController = this->GetComponent<TurtleController>();
	auto transform = this->GetComponent<TurtleController>()->GetParentObject()->GetComponent<TransformComponent>();
	auto animation = this->GetComponent<TurtleController>()->GetParentObject()->GetComponent<AnimationComponent>();
	auto velocity = this->GetComponent<TurtleController>()->GetParentObject()->GetComponent<VelocityComponent>();

	turtleController->Awake();
}
