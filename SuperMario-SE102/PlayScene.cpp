#include <iostream>
#include <fstream>
#include "AssetIDs.h"

#include "PlayScene.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Coin.h"
#include "Platform.h"
#include "Pipe.h"
#include "SampleKeyEventHandler.h"
#include "CBackgroundObject.h"
#include "CoinBrick.h"
#include "Mushroom.h"
#include "MushroomBrick.h"
#include "Koopas.h"
#include "FallPitch.h"
#include "PiranhaPlant.h"
#include "ParaGoomba.h"
#include "SuperLeaf.h"
#include "SuperLeafBrick.h"
#include "SpecialPipe.h"
#include "Bullet.h"
#include "FireTrap.h"
#include "CameraController.h"
#include"CMovingPlatform.h"
using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
	cameraController = NULL;
}

CPlayScene::~CPlayScene()
{
	if (cameraController != NULL)
	{
		delete cameraController;
		cameraController = NULL;
	}
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

CGameObject* CPlayScene::CreateEnemy(int enemyType, float x, float y)
{
	CGameObject* obj = nullptr;

	switch (enemyType)
	{
	case OBJECT_TYPE_GOOMBA:
		obj = new CGoomba(x, y);
		break;

	case OBJECT_TYPE_KOOPAS:
	{
		Koopas* koopas = new Koopas(x, y);
		objectManager.Add(koopas->GetFallSensor());
		obj = koopas;
		break;
	}

	case OBJECT_TYPE_PARAGOOMBA:
		obj = new CParaGoomba(x, y);
		DebugOut(L"[INFO] Creating ParaGoomba at (%f, %f)\n", x, y);
		break;

	case OBJECT_TYPE_FIRETRAP:
		obj = new CFireTrap(x, y);
		DebugOut(L"[INFO] Fire Trap object has been created!\n");
		break;

		// Add other enemy types as needed
	default:
		DebugOut(L"[WARNING] Unknown enemy type: %d\n", enemyType);
		break;
	}

	return obj;
}

void CPlayScene::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPTEXTURE tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return; 
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CPlayScene::_ParseSection_ASSETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring path = ToWSTR(tokens[0]);
	
	LoadAssets(path.c_str());
}

void CPlayScene::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i+1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

/*
	Parse a line in section [OBJECTS] 
*/
void CPlayScene::_ParseSection_OBJECTS(string line, ifstream& f)
{
	vector<string> tokens = split(line);

	// skip invalid lines - an object set must have at least id, x, y
	if (tokens.size() < 3) return;

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player!=NULL) 
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x,y); 
		player = (CMario*)obj;  
		objectManager.AddPlayer(player);
		DebugOut(L"[INFO] Player object has been created!\n");
		break;

	//Enemy Spawn Section

	case OBJECT_TYPE_GOOMBA:
	case OBJECT_TYPE_KOOPAS:
	case OBJECT_TYPE_PARAGOOMBA:
	case OBJECT_TYPE_FIRETRAP:
		obj = CreateEnemy(object_type, x, y);
		break;
	case OBJECT_TYPE_BRICK:
	{
		int brickNumber = 1; 
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		int aniId = ID_ANI_BRICK;
		int bboxWidth = BRICK_BBOX_WIDTH;
		int bboxHeight = BRICK_BBOX_HEIGHT;
		if (tokens.size() >= 6)
		{
			aniId = atoi(tokens[3].c_str());
			bboxWidth = atoi(tokens[4].c_str());
			bboxHeight = atoi(tokens[5].c_str());
		}
		if (tokens.size() >= 8)
		{
			brickNumber = atoi(tokens[6].c_str());
			offsetX = (float)atof(tokens[7].c_str());
			offsetY = (float)atof(tokens[8].c_str());
		}

		obj = new CBrick(x, y, brickNumber, offsetX, offsetY, aniId, bboxWidth, bboxHeight);
		DebugOut(L"[INFO] Brick object has been created!\n");
		break;
	}
	case OBJECT_TYPE_COIN: obj = new CCoin(x, y); break;

	case OBJECT_TYPE_PIPE:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPipe(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);
		DebugOut(L"[INFO] Pipe object has been created!\n");
		break;
	}
	case OBJECT_TYPE_SPECIAL_PIPE:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CSpecialPipe(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);
		DebugOut(L"[INFO] Special Pipe object has been created!\n");
		break;
	}
	case OBJECT_TYPE_PLATFORM:
	{

		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		obj = new CPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end
		);
		DebugOut(L"[INFO] Platform object has been created!\n");
		break;
	}
	case OBJECT_TYPE_BACKGROUND:
	{
		int objectCount = 1;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		int spriteId = atoi(tokens[3].c_str());

		if (tokens.size() >= 7)
		{
			objectCount = atoi(tokens[4].c_str());
			offsetX = (float)atof(tokens[5].c_str());
			offsetY = (float)atof(tokens[6].c_str());
		}

		obj = new CBackgroundObject(x, y, spriteId, objectCount, offsetX, offsetY);
		DebugOut(L"[INFO] Background object has been created!\n");
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		DebugOut(L"[INFO] Portal object has been created!\n");
		break;
	}
	case OBJECT_TYPE_COINBRICK:
	{
		obj = new CCoinBrick(x, y);

		DebugOut(L"[INFO] CoinBrick object has been created!\n");
		break;
	}
	case OBJECT_TYPE_MUSHROOM:
	{
		obj = new CMushroom(x, y);

		DebugOut(L"[INFO] Mushroom object has been created!\n");
		break;
	}
	case OBJECT_TYPE_MUSHROOMBRICK:
	{
		obj = new CMushroomBrick(x, y);

		DebugOut(L"[INFO] MushroomBrick object has been created!\n");
		break;
	}
	case OBJECT_TYPE_FALL_PITCH:
	{
		float w = (float)atof(tokens[3].c_str());
		float h = (float)atof(tokens[4].c_str());
		obj = new CFallPitch(x, y, w, h);
		DebugOut(L"[INFO] FallPitch object has been created!\n");
		break;
	}
	case OBJECT_TYPE_SUPER_LEAF_BRICK:
	{
		obj = new CSuperLeafBrick(x, y);
		DebugOut(L"[INFO] SuperLeafBrick object has been created!\n");
		break;
	}
	case OBJECT_TYPE_CAMERA:
	{
		// Make sure we don't create multiple cameras
		if (cameraController != NULL)
		{
			DebugOut(L"[ERROR] CAMERA object was created before!\n");
			return;
		}

		// Check if we have a camera mode parameter
		int cameraMode = 2;
		if (tokens.size() >= 4)
		{
			cameraMode = atoi(tokens[3].c_str());
		}

		// Create the camera controller
		cameraController = new CameraController(player, CGame::GetInstance());

		// Set the camera mode
		SetCameraMode(cameraMode);

		// Add camera to scene objects for collision detection
		objectManager.Add(cameraController);

		DebugOut(L"[INFO] Camera controller initialized with mode %d\n", cameraMode);
		break;
	}

	case OBJECT_TYPE_CHECKPOINT:
	{
		float width = CHECKPOINT_BBOX_WIDTH;
		float height = CHECKPOINT_BBOX_HEIGHT;

		if (tokens.size() >= 5) {
			width = (float)atof(tokens[3].c_str());
			height = (float)atof(tokens[4].c_str());
		}

		Checkpoint* checkpoint = new Checkpoint(x, y, width, height);
		obj = checkpoint;
		int enemyCount = 0;
		if (tokens.size() >= 6) {
			enemyCount = atoi(tokens[5].c_str());
		}

		// Load enemy definitions
		for (int i = 0; i < enemyCount; i++) {
			char enemyStr[MAX_SCENE_LINE];
			if (!f.getline(enemyStr, MAX_SCENE_LINE))
				break;

			string enemyLine(enemyStr);

			// Skip comments
			if (enemyLine.empty() || enemyLine[0] == '#') {
				i--; // Don't count this as an enemy
				continue;
			}

			vector<string> enemyTokens = split(enemyLine);
			if (enemyTokens.size() < 3)
				continue;

			int enemyType = atoi(enemyTokens[0].c_str());
			float enemyX = (float)atof(enemyTokens[1].c_str());
			float enemyY = (float)atof(enemyTokens[2].c_str());

			CGameObject* enemyObj = CreateEnemy(enemyType, enemyX, enemyY);

			if (enemyObj) {
				enemyObj->SetActive(false);
				objectManager.Add(enemyObj); 
				checkpoint->AddObjectToSpawn(enemyObj);
				DebugOut(L"[INFO] Added enemy type %d to checkpoint\n", enemyType);
			}
		}

		DebugOut(L"[INFO] Checkpoint object has been created with %d enemies!\n", enemyCount);
		break;
	}
	case OBJECT_TYPE_MOVING_PLATFORM:
	{
		float cell_width = (float)atof(tokens[3].c_str());
		float cell_height = (float)atof(tokens[4].c_str());
		int length = atoi(tokens[5].c_str());
		int sprite_begin = atoi(tokens[6].c_str());
		int sprite_middle = atoi(tokens[7].c_str());
		int sprite_end = atoi(tokens[8].c_str());

		// Default speed value
		float moveSpeed = 0.05f;

		// Set custom speed if provided
		if (tokens.size() >= 16)
		{
			moveSpeed = (float)atof(tokens[15].c_str());
		}

		// Create the moving platform with the specified speed
		CMovingPlatform* movingPlatform = new CMovingPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end,
			moveSpeed  // Pass the speed parameter
		);
		// Set movement attributes if provided
		if (tokens.size() >= 13)
		{
			float leftLimit = (float)atof(tokens[9].c_str());
			float rightLimit = (float)atof(tokens[10].c_str());
			float topLimit = (float)atof(tokens[11].c_str());
			float bottomLimit = (float)atof(tokens[12].c_str());

			movingPlatform->SetLimits(leftLimit, rightLimit, topLimit, bottomLimit);
			DebugOut(L"[INFO] Platform limits set to: L=%.2f, R=%.2f, T=%.2f, B=%.2f\n",
				leftLimit, rightLimit, topLimit, bottomLimit);
		}

		// Set movement direction if provided
		if (tokens.size() >= 14)
		{
			int direction = atoi(tokens[13].c_str());
			movingPlatform->SetMoveDirection(direction);
			DebugOut(L"[INFO] Platform direction set to: %d\n", direction);
		}

		// Set auto-movement flag if provided
		if (tokens.size() >= 15)
		{
			bool autoMove = (atoi(tokens[14].c_str()) != 0);
			movingPlatform->SetAutoMoving(autoMove);
			DebugOut(L"[INFO] Platform auto-moving set to: %d\n", autoMove);
		}

		obj = movingPlatform;
		DebugOut(L"[INFO] Moving Platform object has been created!\n");
		break;
	}

	default:
		DebugOut(L"[ERROR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	if (obj)
	{
		obj->SetPosition(x, y);
		if (obj != player)
			objectManager.Add(obj);
	}
}



void CPlayScene::LoadAssets(LPCWSTR assetFile)
{
	DebugOut(L"[INFO] Start loading assets from : %s \n", assetFile);

	ifstream f;
	f.open(assetFile);

	int section = ASSETS_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[SPRITES]") { section = ASSETS_SECTION_SPRITES; continue; };
		if (line == "[ANIMATIONS]") { section = ASSETS_SECTION_ANIMATIONS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case ASSETS_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case ASSETS_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading assets from %s\n", assetFile);
}

void CPlayScene::Load()
{
	DebugOut(L"[INFO] Start loading scene from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;					

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	
		if (line == "[ASSETS]") { section = SCENE_SECTION_ASSETS; continue; };
		if (line == "[OBJECTS]") { section = SCENE_SECTION_OBJECTS; continue; };
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }	

		//
		// data section
		//
		switch (section)
		{ 
			case SCENE_SECTION_ASSETS: _ParseSection_ASSETS(line); break;
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line,f); break;
		}
	}

	f.close();
	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// Update all game objects first
	objectManager.Update(dt);

	if (cameraController == NULL)
	{
		cameraController = new CameraController(player, CGame::GetInstance());
		cameraController->SwitchToThresholdMode(); // Default mode
		objectManager.Add(cameraController);
		DebugOut(L"[INFO] Default camera controller created\n");
	}

	if (cameraController->IsInFreeMove())
	{
		CGame* game = CGame::GetInstance();

		cameraController->SetFreeCameraDirection(FREE_CAM_LEFT, game->IsKeyDown(DIK_LEFT));
		cameraController->SetFreeCameraDirection(FREE_CAM_RIGHT, game->IsKeyDown(DIK_RIGHT));
		cameraController->SetFreeCameraDirection(FREE_CAM_UP, game->IsKeyDown(DIK_UP));
		cameraController->SetFreeCameraDirection(FREE_CAM_DOWN, game->IsKeyDown(DIK_DOWN));
	}

	// Clean up deleted objects
	PurgeDeletedObjects();
}


void CPlayScene::Render()
{
	objectManager.Render();
}

/*
*	Clear all objects from this scene
*/
void CPlayScene::Clear()
{
	objectManager.Clear();
	player = NULL;
}

/*
	Unload scene

	TODO: Beside objects, we need to clean up sprites, animations and textures as well 

*/
void CPlayScene::Unload()
{
	if (cameraController != NULL)
	{
		delete cameraController;
		cameraController = NULL;
	}
	objectManager.Clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::SetCameraMode(int mode)
{
	if (cameraController == NULL)
		return;

	switch (mode)
	{
	case 0:
		cameraController->SwitchToFollowMode();
		DebugOut(L"[INFO] Camera mode set to Follow Player\n");
		break;
	case 1:
		cameraController->SwitchToPushMode();
		DebugOut(L"[INFO] Camera mode set to Push Forward\n");
		break;
	case 2:
		cameraController->SwitchToThresholdMode();
		DebugOut(L"[INFO] Camera mode set to Threshold Based\n");
		break;
	case 3:
		cameraController->SwitchToFreeMove();
		DebugOut(L"[INFO] Camera mode set to Free Move\n");
		break;
	default:
		DebugOut(L"[WARNING] Unknown camera mode: %d\n", mode);
		break;
	}
}

void CPlayScene::PurgeDeletedObjects()
{
	objectManager.PurgeDeletedObjects();
}