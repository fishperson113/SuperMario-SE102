#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

class CStar : public CGameObject
{
public:
	CStar();
	~CStar();

	void Update(DWORD dt);
	void Render();
	void Start();
	void Awake();
private:
};

