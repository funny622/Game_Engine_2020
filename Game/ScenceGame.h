#pragma once
#include "Game.h"
#include "Textures.h"
#include "ScenceManager.h"
#include "GameObject.h"
#include "Brick.h"
#include "Simon.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Map.h"
#include "Candle.h"
#include "Item.h"
#include "Torch.h"
#include "Weapon.h"
#include "Whip.h"


class CScenceGame : public CScenceManager
{
protected:
	Simon * player;					// A play scene has to have player, right? 
	Map * map;
	CWhip * whip;
	CWeapon * weapon;
	vector<LPGAMEOBJECT> objects;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);


public:
	CScenceGame(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();
	virtual void RemoveObjects();
	Simon * GetPlayer() { return player; }
	CWhip * GetWhip() { return whip; }
	CWeapon * GetWeapon() { return weapon; }
	void WhipUpdate(DWORD dt, int x, int y);
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{
public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode) {};
	CPlayScenceKeyHandler(CScenceManager *s) :CScenceKeyHandler(s) {};
};

