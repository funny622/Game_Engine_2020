#include "Weapon.h"

CWeapon::CWeapon()
{
	isFinish = 1;
}


CWeapon::~CWeapon()
{
}

void CWeapon::Attack(float X, float Y, int Direction)
{
	this->x = X;
	this->y = Y;
	this->nx = Direction;
	isFinish = false; // chưa kết thúc


	LastTimeAttack = GetTickCount(); // lưu lại thời điểm lúc vừa tấn công, làm đánh dấu tránh 1 hit đánh nhiều lần cho các object, có health >1.
}

void CWeapon::Render()
{
	if (isFinish)
		return;

	RenderBoundingBox();
}


bool CWeapon::isCollision(CGameObject* obj)
{
	if (isFinish == true)
		return false;

	// dt, dx, dy đã update 
	if (obj->GetHealth() <= 0) // vật này die rồi thì ko va chạm
		return false;
	return isCollitionObjectWithObject(obj);
}



bool CWeapon::GetFinish()
{
	return isFinish;
}

void CWeapon::SetFinish(bool b)
{
	isFinish = b;
}

DWORD CWeapon::GetLastTimeAttack()
{
	return LastTimeAttack;
}