#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scene.h"
#include "GameObject.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "ObjectManager.h"
#include "CameraController.h"


class CPlayScene: public CScene
{
	CGameObject* CreateEnemy(int enemyType, float x, float y);
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	ObjectManager objectManager;
	CameraController* cameraController;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line, ifstream& f);
	void LoadCamera();
	void LoadAssets(LPCWSTR assetFile);
	
public: 
	CPlayScene(int id, LPCWSTR filePath);
	~CPlayScene();
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	LPGAMEOBJECT GetPlayer() { return player; }

	void Clear();
	void PurgeDeletedObjects();
	ObjectManager* GetObjectManager() { return &objectManager; }
	static bool IsGameObjectDeleted(const LPGAMEOBJECT& o);
	void SetCameraMode(int mode); // 0 = Follow, 1 = Push, 2 = Threshold

};

typedef CPlayScene* LPPLAYSCENE;

