#include <iostream>
#include <fstream>
#include "AssetIDs.h"
#include "HUD.h"

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
#include "CMovingPlatform.h"
#include "KoopaParatroopa.h"
#include "HitBox.h"
#include "BoomerangBro.h"
#include "Boomerang.h"
#include "Card.h"
#include "Switch.h"
#include "SwitchBrick.h"

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
	cameraController = NULL;
	isPaused = false;
	timeLimit = DEFAULT_TIME_LIMIT;
	lastTick = GetTickCount64();
	isGameOver = false;
	gameOverStart = 0;
}

CPlayScene::~CPlayScene()
{
	if (cameraController != NULL)
	{
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

	case OBJECT_TYPE_PARAGOOMBA:
		obj = new CParaGoomba(x, y);
		DebugOut(L"[INFO] Creating ParaGoomba at (%f, %f)\n", x, y);
		break;

	case OBJECT_TYPE_KOOPA_PARATROOPA:
		obj = new Koopas(x, y, 2);
		DebugOut(L"[INFO] Koopa Paratroopa object has been created!\n");
		break;
	case OBJECT_TYPE_KOOPAS:
	{
		Koopas* koopas = new Koopas(x, y, 0);
		objectManager.Add(koopas->GetFallSensor());
		obj = koopas;
		DebugOut(L"[INFO] Koopas object has been created!\n");
		break;
	}
	case OBJECT_TYPE_RED_WINGS_KOOPAS:
	{
		obj = new Koopas(x, y, 3);
		DebugOut(L"[INFO] Red Wings Koopas object has been created!\n");
		break;
	}
	case OBJECT_TYPE_KOOPATROOPA:
	{
		obj = new Koopas(x, y, 1);
		DebugOut(L"[INFO] KoopaTroopa object has been created!\n");
		break;
	}
	case OBJECT_TYPE_BOOMERANG_BRO:
	{
		CBoomerangBro* boomerangBro = new CBoomerangBro(x, y);
		objectManager.Add(boomerangBro->GetBoomerang());
		obj = boomerangBro;
		DebugOut(L"[INFO] Boomerang Bro object has been created!\n");
		break;
	}
	case OBJECT_TYPE_RED_FIRETRAP:
	{
		obj = new CFireTrap(x, y, 0);
		DebugOut(L"[INFO] Fire Trap object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GREEN_PIRANHA:
	{
		obj = new CFireTrap(x, y, 2);
		DebugOut(L"[INFO] Fire Trap object has been created!\n");
		break;
	}
	case OBJECT_TYPE_GREEN_FIRETRAP:
	{
		obj = new CFireTrap(x, y, 1);
		DebugOut(L"[INFO] Fire Trap object has been created!\n");
		break;
	}
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
		{  // Add a new scope with curly braces
			obj = new CMario(x, y);
			player = (CMario*)obj;
			HitBox* hitbox = new HitBox(player);
			player->SetHitbox(hitbox);
			objectManager.AddPlayer(player);
			objectManager.Add(hitbox);
			DebugOut(L"[INFO] Player object has been created!\n");
		}
		break;

	//Enemy Spawn Section

	case OBJECT_TYPE_KOOPAS:
	case OBJECT_TYPE_RED_WINGS_KOOPAS:
	case OBJECT_TYPE_KOOPATROOPA:
	case OBJECT_TYPE_GOOMBA:
	case OBJECT_TYPE_PARAGOOMBA:
	case OBJECT_TYPE_KOOPA_PARATROOPA:
	case OBJECT_TYPE_BOOMERANG_BRO:
	case OBJECT_TYPE_RED_FIRETRAP:
	case OBJECT_TYPE_GREEN_FIRETRAP:
	case OBJECT_TYPE_GREEN_PIRANHA:
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
		bool isBreakable = false;
		int coinCount = 0;
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
		if (tokens.size() >= 10)
		{
			isBreakable = atoi(tokens[9].c_str()) != 0; // Convert to boolean
			coinCount = atoi(tokens[10].c_str());
		}

		obj = new CBrick(x, y, brickNumber, offsetX, offsetY, aniId, bboxWidth, bboxHeight, isBreakable, coinCount);
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

		// Add pipe type parameter (default to standard)
		int pipeType = PIPE_TYPE_STANDARD;
		if (tokens.size() >= 10)
		{
			pipeType = atoi(tokens[9].c_str());
		}

		CPipe* pipe = new CPipe(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end,
			pipeType
		);

		// Check if it's a teleport pipe
		if (tokens.size() >= 13)
		{
			// Entry Direction: 0=UP, 1=DOWN, 2=LEFT, 3=RIGHT
			int entryDirection = atoi(tokens[10].c_str());
			float target_x = (float)atof(tokens[11].c_str());
			float target_y = (float)atof(tokens[12].c_str());

			// Default exit direction (if not specified) is UP
			int exitDirection = 0; // UP
			if (tokens.size() >= 14)
			{
				exitDirection = atoi(tokens[13].c_str());
			}

			PipeDirection entryDir, exitDir;

			// Convert entry direction
			switch (entryDirection)
			{
			case 0: entryDir = PipeDirection::UP; break;
			case 1: entryDir = PipeDirection::DOWN; break;
			case 2: entryDir = PipeDirection::LEFT; break;
			case 3: entryDir = PipeDirection::RIGHT; break;
			default: entryDir = PipeDirection::DOWN; break;
			}

			// Convert exit direction
			switch (exitDirection)
			{
			case 0: exitDir = PipeDirection::UP; break;
			case 1: exitDir = PipeDirection::DOWN; break;
			case 2: exitDir = PipeDirection::LEFT; break;
			case 3: exitDir = PipeDirection::RIGHT; break;
			default: exitDir = PipeDirection::UP; break;
			}

			pipe->SetAsEntrance(target_x, target_y, entryDir, exitDir);
			DebugOut(L"[INFO] Teleport Pipe created with target (%f, %f), entry dir %d, exit dir %d\n",
				target_x, target_y, pipe->GetEntryDirection(), pipe->GetExitDirection());
		}
		// Check if it's an exit pipe
		else if (tokens.size() >= 11 && atoi(tokens[10].c_str()) == 1)
		{
			pipe->SetAsExit();
			DebugOut(L"[INFO] Exit Pipe created\n");
		}

		obj = pipe;
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
		int breakCount = 1;

		if (tokens.size() >= 4)
		{
			breakCount = atoi(tokens[3].c_str());
		}

		obj = new CCoinBrick(x, y, false, breakCount);

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
	case OBJECT_TYPE_SWITCH:
	{
		obj = new CSwitch(x, y);
		DebugOut(L"[INFO] Switch object has been created!\n");
		break;
	}
	case OBJECT_TYPE_SWITCH_BRICK:
	{
		obj = new CSwitchBrick(x, y);
		DebugOut(L"[INFO] Switch Brick object has been created!\n");
		break;
	}
	case OBJECT_TYPE_FIRETRAP:
	{
		int type = atoi(tokens[3].c_str());
		obj = new CFireTrap(x, y, type);
		DebugOut(L"[INFO] Fire Trap object has been created!\n");
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
		int platformCount = 0;
		if (tokens.size() >= 6) {
			enemyCount = atoi(tokens[5].c_str());
		}
		if (tokens.size() >= 7) {
			platformCount = atoi(tokens[6].c_str());
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

		for (int i = 0; i < platformCount; i++) {
			char platformStr[MAX_SCENE_LINE];
			if (!f.getline(platformStr, MAX_SCENE_LINE))
				break;

			string platformLine(platformStr);

			// Skip comments
			if (platformLine.empty() || platformLine[0] == '#') {
				i--; // Don't count this as a platform
				continue;
			}

			vector<string> platformTokens = split(platformLine);
			if (platformTokens.size() < 9) // Need at least basic parameters for a platform
				continue;

			// Get platform parameters
			int objectType = atoi(platformTokens[0].c_str());
			float platformX = (float)atof(platformTokens[1].c_str());
			float platformY = (float)atof(platformTokens[2].c_str());

			// Only process if it's a moving platform
			if (objectType == OBJECT_TYPE_MOVING_PLATFORM) {
				float cell_width = (float)atof(platformTokens[3].c_str());
				float cell_height = (float)atof(platformTokens[4].c_str());
				int length = atoi(platformTokens[5].c_str());
				int sprite_begin = atoi(platformTokens[6].c_str());
				int sprite_middle = atoi(platformTokens[7].c_str());
				int sprite_end = atoi(platformTokens[8].c_str());

				// Default speed
				float moveSpeed = 0.05f;
				if (platformTokens.size() >= 10) {
					moveSpeed = (float)atof(platformTokens[9].c_str());
				}

				// Create the platform
				CMovingPlatform* platform = new CMovingPlatform(
					platformX, platformY,
					cell_width, cell_height, length,
					sprite_begin, sprite_middle, sprite_end,
					moveSpeed
				);

				if (platform) {
					platform->SetActive(false); // Initially inactive
					objectManager.Add(platform);
					checkpoint->AddObjectToSpawn(platform);
					DebugOut(L"[INFO] Added moving platform at (%f, %f) to checkpoint\n", platformX, platformY);
				}
			}
		}

		DebugOut(L"[INFO] Checkpoint object has been created with %d enemies and %d platforms!\n",
			enemyCount, platformCount);
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
		// Check if this platform belongs to a checkpoint (optional parameter)
		bool isCheckpointControlled = false;
		if (tokens.size() >= 11) {
			isCheckpointControlled = atoi(tokens[10].c_str()) != 0;
			return; // Skip this object if it's checkpoint controlled
		}

		// Default speed value
		float moveSpeed = 0.05f;

		// Set custom speed if provided (optional parameter)
		if (tokens.size() >= 10)
		{
			moveSpeed = (float)atof(tokens[9].c_str());
		}

		// Create the moving platform with the specified parameters
		CMovingPlatform* platform = new CMovingPlatform(
			x, y,
			cell_width, cell_height, length,
			sprite_begin, sprite_middle, sprite_end,
			moveSpeed
		);

		obj = platform;
		DebugOut(L"[INFO] Moving Platform object has been created! Active: %d\n", !isCheckpointControlled);
		break;
	}
	case OBJECT_TYPE_BREAKABLE_BRICK:
	{
		int brickNumber = 1;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		int aniId = ID_ANI_BRICK;
		int bboxWidth = BRICK_BBOX_WIDTH;
		int bboxHeight = BRICK_BBOX_HEIGHT;
		bool isBreakable = true;
		int coinCount = 0;
		if (tokens.size() >= 6)
		{
			aniId = atoi(tokens[3].c_str());
			bboxWidth = atoi(tokens[4].c_str());
			bboxHeight = atoi(tokens[5].c_str());
		}

		CBrick* brick = new CBrick(x, y, brickNumber, offsetX, offsetY, aniId, bboxWidth, bboxHeight, isBreakable, coinCount);
		obj = brick;
		DebugOut(L"[INFO] Brick object has been created!\n");
		break;
	}
	case OBJECT_TYPE_BREAKABLE_BRICK_TURN_COIN:
	{
		int brickNumber = 1;
		float offsetX = 0.0f;
		float offsetY = 0.0f;
		int aniId = ID_ANI_BRICK;
		int bboxWidth = BRICK_BBOX_WIDTH;
		int bboxHeight = BRICK_BBOX_HEIGHT;
		bool isBreakable = true;
		int coinCount = 0;
		if (tokens.size() >= 6)
		{
			aniId = atoi(tokens[3].c_str());
			bboxWidth = atoi(tokens[4].c_str());
			bboxHeight = atoi(tokens[5].c_str());
		}

		CBrick* brick = new CBrick(x, y, brickNumber, offsetX, offsetY, aniId, bboxWidth, bboxHeight, isBreakable, coinCount);
		brick->SetAbleToChangeToCoin(true);
		obj = brick;
		break;
	}
	case OBJECT_TYPE_CARD:
	{
		obj = new Card(x, y);
		DebugOut(L"[INFO] Card object has been created at (%f, %f)!\n", x, y);
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
	if (isPaused) return;
	UpdateTimer();
	if (isGameOver)
	{
		ULONGLONG now = GetTickCount64();
		if (now - gameOverStart > GAME_OVER_DELAY)
		{
			return;
		}
	}
	// Update all game objects first
	objectManager.Update(dt);
	HUD::GetInstance()->Update();

	if (cameraController == NULL)
	{
		cameraController = new CameraController(player, CGame::GetInstance());
		cameraController->SwitchToThresholdMode(); // Default mode
		//cameraController->SwitchToFreeMove();
		objectManager.Add(cameraController);
		DebugOut(L"[INFO] Default camera controller created\n");
	}

	if (cameraController->IsInFreeMove())
	{
		CGame* game = CGame::GetInstance();

		cameraController->SetFreeCameraDirection(FREE_CAM_LEFT, game->IsKeyDown(DIK_LEFT));
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
	HUD::GetInstance()->Render();
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

void CPlayScene::UpdateTimer()
{
	ULONGLONG now = GetTickCount64();
	if (now - lastTick >= 1000 && !isGameOver && timeLimit > 0)
	{
		timeLimit--;
		lastTick = now;

		if (timeLimit <= 0)
		{
			TriggerGameOver();
		}
	}
}

void CPlayScene::TriggerGameOver()
{
	if (isGameOver) return; 

	isGameOver = true;
	gameOverStart = GetTickCount64();

	// Kill Mario if time ran out
	CMario* mario = dynamic_cast<CMario*>(player);
	if (mario && timeLimit <= 0 && mario->GetState() != MARIO_STATE_DIE)
	{
		mario->SetState(MARIO_STATE_DIE);
	}
}

void CPlayScene::Reload()
{
	DebugOut(L"[INFO] Reloading scene %d\n", id);

	// Save important information before unloading
	int currentSceneId = this->id;
	LPCWSTR currentFilePath = this->sceneFilePath;

	// Reset game state
	isPaused = false;
	isGameOver = false;
	timeLimit = DEFAULT_TIME_LIMIT;
	lastTick = GetTickCount64();

	// Reset HUD stats
	HUD* hud = HUD::GetInstance();
	if (hud)
	{
		// Reset coin counter
		hud->SetLastCoin(0);

		// Reset score/points
		hud->SetLastPoints(0);

		// Reset card collection
		//Card::ResetCollectedCards();
		//Card::ResetLastCollectedType();

		// Reset other HUD states
		hud->SetInitCard(false);

		// Reset HUD drawing flags
		HUD::isStarting = false;
		HUD::initStart = false;
		HUD::isAllowToRenderHudStart = false;
	}
	Clear();

	Unload();

	CSprites::GetInstance()->Clear();
	CAnimations::GetInstance()->Clear();

	key_handler = new CSampleKeyHandler(this);
	
	player = NULL;
	cameraController = NULL;

	Load();

	DebugOut(L"[INFO] Scene %d reloaded successfully!\n", id);

}

void CPlayScene::PurgeDeletedObjects()
{
	objectManager.PurgeDeletedObjects();
}