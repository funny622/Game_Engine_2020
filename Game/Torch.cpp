#include "Torch.h"

void CTorch::Render()
{
	if (!isHitted)
		animation_set->at(0)->Render(x, y);
	RenderBoundingBox();
}
void CTorch::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{

}

void CTorch::SetHit()
{
	isHitted = true;
}

void CTorch::GetBoundingBox(float& l, float& t, float& r, float& b)
{
	l = x;
	t = y;
	r = x + TORCH_BBOX_WIDTH;
	b = y + TORCH_BBOX_HEIGHT;
}
