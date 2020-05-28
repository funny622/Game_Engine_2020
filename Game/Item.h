#pragma once
#include "GameObject.h"
#include <unordered_map>

#define ITEM_GRAVITY		0.002f

// Item
#define ITEM_WHIP						300
#define ITEM_HEART						301
#define ITEM_MONEY						302
#define ITEM_KNIFE						303
#define ITEM_MINIHEART					304

// Heart
#define HEART_BBOX_WIDTH				12
#define HEART_BBOX_HEIGHT				10

// Mini heart
#define MINIHEART_BBOX_WIDTH				8
#define MINIHEART_BBOX_HEIGHT				8

// Whip item
#define WHIPITEM_BBOX_WIDTH				16
#define WHIPITEM_BBOX_HEIGHT			16

//Knife
#define KNIFE_BBOX_WIDTH				16
#define KNIFE_BBOX_HEIGHT				9

//Money
#define MONEY_BBOX_WIDTH				15
#define MONEY_BBOX_HEIGHT				15

using namespace std;

class CItem : public CGameObject
{
public:
	int type;
	bool isEaten;
public:
	void SetType(int _type) { type = _type; }
	int GetType() { return type; }
	void SetEaten() { isEaten = true; }
	bool GetEaten() { return isEaten; }
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects = NULL);
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
};