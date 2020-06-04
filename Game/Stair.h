#pragma once

#include "GameObject.h"

#define STAIR_WALKING_SPEED	0.03f;

#define STAIR_BBOX_WIDTH		32
#define STAIR_BBOX_HEIGHT	8

class CStair : public CGameObject
{
public:
	float minX, maxX;

	CStair();
	CStair(float minX, float maxX) {
		this->minX = minX;
		this->maxX = maxX;
		this->vx = STAIR_WALKING_SPEED;
	}

	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects);
	virtual void Render();

};