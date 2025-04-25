#pragma once
#include "GameObject.h"
class CFallPitch : public CGameObject
{
public:
	CFallPitch(float x, float y, float width, float height);
	void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	void Render() {}
	void Update(DWORD dt) {}
	~CFallPitch() {}
private:
	float width;
	float height;
};

