#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

class CBrick : public CGameObject
{
public:
	CBrick();
	~CBrick();

	void Update(DWORD dt);
	void Render();
	void Start();
	void Awake();
private:
};

