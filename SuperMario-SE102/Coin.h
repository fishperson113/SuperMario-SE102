#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

class CCoin : public CGameObject
{
public:
	CCoin();
	~CCoin();

	void Update(DWORD dt);
	void Render();
	void Start();
	void Awake();
private:
};

