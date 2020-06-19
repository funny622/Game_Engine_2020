#pragma once
#include "GameObject.h"

#define CANDLE_BBOX_WIDTH  8
#define CANDLE_BBOX_HEIGHT 16
#define CANDLE_STATE_DESTROYED 300

class CCandle : public CGameObject
{
public:
	bool isHitted;
	int itemID;
	CCandle();
	CCandle(int itemID) :CGameObject() {
		isHitted = false;
		this->itemID = itemID;
	}
	void SetHit();
	virtual void Render();
	virtual void GetBoundingBox(float& l, float& t, float& r, float& b);
	void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	void SetPosition(float x, float y)
	{
		this->x = x;
		this->y = y;
	}
};

