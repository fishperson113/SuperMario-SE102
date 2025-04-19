#pragma once
#include "GameObject.h"
#include "Component.h"

class CPortal : public CGameObject
{
	int scene_id;	// target scene to switch to 

	float width;
	float height;
public:
	CPortal(float l, float t, float r, float b, int scene_id);
	~CPortal();
	int GetSceneId() { return this->scene_id; }
};

