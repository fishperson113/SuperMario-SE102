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
#define DEFAULT_TIME_LIMIT 300 
#define GAME_OVER_DELAY 3000
class CPlayScene: public CScene
{
	CGameObject* CreateEnemy(int enemyType, float x, float y);
protected: 
	// A play scene has to have player, right? 
	LPGAMEOBJECT player;					

	ObjectManager objectManager;
	CameraController* cameraController;
	bool isPaused;

	int timeLimit;         
	ULONGLONG lastTick;    
	bool isGameOver;       
	ULONGLONG gameOverStart;

	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);

	void _ParseSection_ASSETS(string line);
	void _ParseSection_OBJECTS(string line, ifstream& f);
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
	bool IsPaused() const { return isPaused; }
	void SetPaused(bool paused) { isPaused = paused; }
	void TogglePause() { isPaused = !isPaused; }
	int GetTimeRemaining() const { return timeLimit; }
	void SetTimeLimit(int seconds) { timeLimit = seconds; }
	void UpdateTimer();
	bool IsGameOver() const { return isGameOver; }
	void TriggerGameOver();
	void Reload();

};

typedef CPlayScene* LPPLAYSCENE;

