#include <iostream>
#include <fstream>

#include "ScenceGame.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "Weapon.h"
#include "Whip.h"

using namespace std;

CScenceGame::CScenceGame(int id, LPCWSTR filePath) :
	CScenceManager(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
Load scene resources from scene file (textures, sprites, animations and objects)
See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_MAP 1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_SIMON	0
#define OBJECT_TYPE_BRICK	1
#define OBJECT_TYPE_GOOMBA	2
#define OBJECT_TYPE_KOOPAS	3
#define OBJECT_TYPE_CANDLE	4
#define OBJECT_TYPE_TORCH	5
#define OBJECT_TYPE_WHIP	20
#define OBJECT_TYPE_ITEM	30

#define OBJECT_TYPE_WHIP_ANI	24

#define OBJECT_TYPE_PORTAL	50
#define MAP_1 1000
#define MAP_2 1100
#define MAP_3 1200

#define MAX_SCENE_LINE 1024


void CScenceGame::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CScenceGame::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CScenceGame::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CScenceGame::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

void CScenceGame::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	wstring filePath_texture = ToWSTR(tokens[1]);
	wstring filePath_data = ToWSTR(tokens[2]);
	int mapRows = atoi(tokens[3].c_str());
	int mapColumns = atoi(tokens[4].c_str());
	int tileRows = atoi(tokens[5].c_str());
	int tileColumns = atoi(tokens[6].c_str());
	int tileWidth = atoi(tokens[7].c_str());
	int tileHeight = atoi(tokens[8].c_str());

	map = new Map(ID, filePath_texture.c_str(), filePath_data.c_str(), mapRows, mapColumns, tileRows, tileColumns, tileWidth, tileHeight);
}
/*
Parse a line in section [OBJECTS]
*/
void CScenceGame::_ParseSection_OBJECTS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_SIMON:
		if (player != NULL)
		{
			return;
		}
		obj = new Simon(x, y);
		player = (Simon*)obj;
		whip = new CWhip();
		break;
	case OBJECT_TYPE_GOOMBA: obj = new CGoomba(); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS: obj = new CKoopas(); break;
	case OBJECT_TYPE_TORCH:
	{
		int itemID = atoi(tokens[4].c_str());
		obj = new CTorch(itemID);
		break;
	}
	case OBJECT_TYPE_CANDLE:
	{
		int itemID = atoi(tokens[4].c_str());
		obj = new CCandle(itemID);
		break;
	}
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
	default:
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

	LPANIMATION_SET ani_set_whip = animation_sets->Get(OBJECT_TYPE_WHIP_ANI);
	whip->SetAnimationSet(ani_set_whip);
}

void CScenceGame::Load()
{
	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line == "[MAP]") {
			section = SCENE_SECTION_MAP; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));
}

void CScenceGame::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 1; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow simon
	float cx, cy;
	player->GetPosition(cx, cy);

	CGame* game = CGame::GetInstance();
	cx += game->GetScreenWidth() / 2;
	cy -= game->GetScreenHeight() / 2;

	if (cx < 0)
	{
		cx = 0;
	}

	CGame::GetInstance()->SetCamPos(cx, 0.0f /*cy*/);

	if (player->x > (SCREEN_WIDTH / 2) && player->x + (SCREEN_WIDTH / 2) < map->GetWidthMap())
	{
		cx = player->x - (SCREEN_WIDTH / 2);
		CGame::GetInstance()->SetCamPos(cx, 0.0f);
	}

	//if (weapon->x < cx + SCREEN_WIDTH && weapon->x > cx && weapon->y < cy + SCREEN_HEIGHT && weapon->isEnable())
	//{
	//	coObjects.push_back(player);
	//	weapon->Update(dt, &coObjects);
	//}
	//else
	//{
	//	weapon->enable = false;
	//}
}

void CScenceGame::Render()
{
	map->Draw();
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	if (player->GetState() == SIMON_STATE_HIT)
	{
		if (player->GetIsThrowWeapon() == false && whip != NULL)
		{
			whip->nx = player->nx;
			whip->Render();
		}
	}
	else whip->enable = false;
}

/*
Unload current scene
*/
void CScenceGame::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	Simon *simon = ((CScenceGame*)scence)->GetPlayer();
	CWhip* whip = ((CScenceGame*)scence)->GetWhip();
	CWeapon* weapon = ((CScenceGame*)scence)->GetWeapon();
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (simon->GetState() == SIMON_STATE_JUMP)
		{
			return;
		}
		else
		{
			simon->SetState(SIMON_STATE_JUMP);
		}
		break;
	case DIK_Z:
		whip->enable = true;
		whip->StartHit();
		if (simon->GetState() == SIMON_STATE_HIT)
		{
			return;
		}
		simon->SetIsThrowWeapon(false);
		simon->SetState(SIMON_STATE_HIT);
		break;
	case DIK_A:
		simon->Reset();
		break;
	}
}

void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	Simon *simon = ((CScenceGame*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (simon->GetState() == SIMON_STATE_DIE) return;
	if (simon->GetState() == SIMON_STATE_JUMP && !simon->CheckStandGround())
	{
		return;
	}
	if (game->IsKeyDown(DIK_RIGHT))	
	{
		simon->SetOrientation(1);
		simon->SetState(SIMON_STATE_WALKING);
		simon->SetIsStand(false);
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		simon->SetOrientation(-1);
		simon->SetState(SIMON_STATE_WALKING);
		simon->SetIsStand(false);
	}
	else
	{
		simon->SetState(SIMON_STATE_IDLE);
		simon->SetIsStand(false);
	}
}

void CScenceGame::RemoveObjects()
{
	for (int i = 0; i < objects.size(); i++)
	{
		if (dynamic_cast<CTorch*>(objects.at(i)))
		{
			CTorch* torch = dynamic_cast<CTorch*>(objects.at(i));
			if (torch->isHitted)
			{
				float torch_x, torch_y, torch_right, torch_bottom;
				torch->GetBoundingBox(torch_x, torch_y, torch_right, torch_bottom);

				CItem* item = new CItem();
				item->SetPosition(torch_x, torch_y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);

				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set;

				ani_set = animation_sets->Get(torch->itemID);
				item->SetType(torch->itemID);
				item->SetAnimationSet(ani_set);

				objects.erase(objects.begin() + i);
				delete torch;
			}
		}
		else if (dynamic_cast<CCandle*>(objects.at(i))) {
			CCandle* candle = dynamic_cast<CCandle*>(objects.at(i));
			if (candle->isHitted)
			{
				float x, y, r, b;
				candle->GetBoundingBox(x, y, r, b);

				CItem* item = new CItem();
				item->SetPosition(x, y);
				item->SetSpeed(0, -0.1);
				objects.push_back(item);

				CAnimationSets* animation_sets = CAnimationSets::GetInstance();
				LPANIMATION_SET ani_set;

				ani_set = animation_sets->Get(candle->itemID);
				item->SetType(candle->itemID);
				item->SetAnimationSet(ani_set);

				objects.erase(objects.begin() + i);
				delete candle;
			}
		}
		else if (dynamic_cast<CItem*>(objects.at(i)))
		{
			CItem* item = dynamic_cast<CItem*>(objects.at(i));

			if (item->GetEaten())
			{
				objects.erase(objects.begin() + i);
				delete item;
			}
		}
	}
}