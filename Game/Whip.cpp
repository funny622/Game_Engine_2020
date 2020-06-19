#include "Whip.h"
#include "Torch.h"

CWhip::CWhip()
{
}


CWhip::~CWhip()
{
}

void CWhip::Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects)
{
	CGameObject::Update(dt, coObjects);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);

		if (dynamic_cast<CTorch*>(obj))
		{
			float left, top, right, bottom;
			obj->GetBoundingBox(left, top, right, bottom);

			if (GetTickCount() - isRender > 200 && CheckCollision(left, top, right, bottom) == true)
			{
				if (this->enable)
				{
					obj->SetState(TORCH_STATE_DESTROYED);
				}
			}
		}
	}
}

void CWhip::Render()
{
	if (this->nx == -1) animation_set->at(0)->Render(x, y);
	else animation_set->at(1)->Render(x, y);

	RenderBoundingBox();
}

void CWhip::SetWhipPosition(int simonX, int simonY, bool isStand)
{
	this->x = simonX - 90.0f;
	this->y = simonY;
	if (!isStand)
	{
		this->y = simonY + 15.0f;
	}
}

void CWhip::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	if (this->nx == -1)
	{
		left = x + 50;
		top = y + 15;
		right = left + WHIP_BBOX_WIDTH;
		bottom = top + WHIP_BBOX_HEIGHT;
	}
	else {
		top = y + 15;
		bottom = top + WHIP_BBOX_HEIGHT;
		right = x + 190;
		left = right - WHIP_BBOX_WIDTH;
	}
}

bool CWhip::CheckCollision(float obj_left, float obj_top, float obj_right, float obj_bottom)
{
	float whip_left,
		whip_top,
		whip_right,
		whip_bottom;

	GetBoundingBox(whip_left, whip_top, whip_right, whip_bottom);

	//return CGameObject::isCollitionObjectWithObject(whip_left, whip_top, whip_right, whip_bottom, obj_left, obj_top, obj_right, obj_bottom);
	return true;
}

void CWhip::StartHit()
{
	isRender = GetTickCount();
}