#pragma once

#include "Utils.h"
#include "GameObject.h"
#include "Item.h"


class CWeapon : public CGameObject
{
protected:
	bool isFinish;
	DWORD LastTimeAttack; // Thời điểm tấn công cuối cùng

public:
	CWeapon();
	~CWeapon();

	int GetDirection();
	void SetDirection(int Direction);

	virtual void Attack(float X, float Y, int Direction);


	virtual void Render();
	virtual bool isCollision(CGameObject* obj); // kiểm tra vũ khí này có va chạm với object kia hay không?
	virtual void RenderIcon(float X, float Y) = 0;
	bool GetFinish();

	void SetFinish(bool b);

	DWORD GetLastTimeAttack();
};
