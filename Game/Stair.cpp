#include "Stair.h"

CStair::CStair()
{
	minX = 0;
	maxX = 290;
}

void CStair::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = x;
	top = y;
	right = x + STAIR_BBOX_WIDTH;
	bottom = y + STAIR_BBOX_HEIGHT;
}

void CStair::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	x += dx;
	y += dy;

	if (vx < 0 && x < minX) {
		x = minX;
		vx = -vx;
	}

	if (vx > 0 && x + STAIR_BBOX_WIDTH > maxX) {
		x = maxX - STAIR_BBOX_WIDTH;
		vx = -vx;
	}
}

void CStair::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
