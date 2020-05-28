#include "Candle.h"

void CCandle::Render()
{
	animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}

void CCandle::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
}

void CCandle::SetHit()
{
	isHitted = true;
}

void CCandle::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + CANDLE_BBOX_WIDTH;
	b = y + CANDLE_BBOX_HEIGHT;
}
