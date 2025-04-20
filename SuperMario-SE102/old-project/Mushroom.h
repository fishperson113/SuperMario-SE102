#pragma once
#include "GameObject.h"
#include "Animation.h"
#include "Animations.h"
#include "AssetIDs.h"

#define	MUSHROOM_WIDTH 10
#define MUSHROOM_BBOX_WIDTH 10
#define MUSHROOM_BBOX_HEIGHT 16

class CMushroom : public CGameObject
{
public:
	CMushroom();
	~CMushroom();

	void Update(DWORD dt);
	void Render();
	void Start();
	void Awake();
private:
	void Move();
};

