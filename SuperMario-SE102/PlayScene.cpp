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

using namespace std;

CPlayScene::CPlayScene(int id, LPCWSTR filePath):
	CScene(id, filePath)
{
	player = NULL;
	key_handler = new CSampleKeyHandler(this);
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_ASSETS	1
#define SCENE_SECTION_OBJECTS	2

#define ASSETS_SECTION_UNKNOWN -1
#define ASSETS_SECTION_SPRITES 1
#define ASSETS_SECTION_ANIMATIONS 2

#define MAX_SCENE_LINE 1024

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
void CPlayScene::_ParseSection_OBJECTS(string line)
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
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(x,y); break;
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
	/*case OBJECT_TYPE_PORTAL:
	{
		float r = (float)atof(tokens[3].c_str());
		float b = (float)atof(tokens[4].c_str());
		int scene_id = atoi(tokens[5].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
		break;
	}*/
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
	case OBJECT_TYPE_KOOPAS:
	{
		Koopas* koopas = new Koopas(x, y);
		obj = koopas;
		objectManager.Add(koopas->GetFallSensor());
		break;
	}
	case OBJECT_TYPE_PARAGOOMBA:
	{
		obj = new CParaGoomba(x, y);
		DebugOut(L"[INFO] ParaGoomba object has been created!\n");
		break;
	}
	case OBJECT_TYPE_PIRANHAPLANT:
	{
		obj = new CPiranhaPlant(x, y);
		DebugOut(L"[INFO] PiranhaPlant object has been created!\n");
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
			case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	DebugOut(L"[INFO] Done loading scene  %s\n", sceneFilePath);
}

void CPlayScene::Update(DWORD dt)
{
	// Update all game objects first
	objectManager.Update(dt);

	// Control camera position
	float cx = 0, cy = 0;
	CGame* game = CGame::GetInstance();

	// If player exists, follow player
	if (player != NULL) {
		player->GetPosition(cx, cy);

		// Center the camera on the player
		cx -= game->GetBackBufferWidth() / 2;
		cy -= game->GetBackBufferHeight() / 2;

		game->SetCamPos(cx, 20);
	}
	// If no player exists, allow manual camera control
	else {
		// Get current camera position
		game->GetCamPos(cx, cy);

		// Move camera based on keyboard input
		if (game->IsKeyDown(DIK_RIGHT))
			cx += 0.5f * dt;
		if (game->IsKeyDown(DIK_LEFT))
			cx -= 0.5f * dt;
		if (game->IsKeyDown(DIK_DOWN))
			cy += 0.5f * dt;
		if (game->IsKeyDown(DIK_UP))
			cy -= 0.5f * dt;

		game->SetCamPos(cx, cy);
	}

	// Constrain camera to not go below 0
	if (cx < 0) cx = 0;

	if (cy < 0) cy = 0;
	// Update camera position

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
	objectManager.Clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %d unloaded! \n", id);
}

bool CPlayScene::IsGameObjectDeleted(const LPGAMEOBJECT& o) { return o == NULL; }

void CPlayScene::PurgeDeletedObjects()
{
	objectManager.PurgeDeletedObjects();
}