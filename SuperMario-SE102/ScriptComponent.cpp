#include"Component.h"

void ScriptComponent::OnCollisionWith(LPCOLLISIONEVENT e)
{
   if (!e->obj->GetComponent<ColliderComponent>()->IsBlocking()) return;
};