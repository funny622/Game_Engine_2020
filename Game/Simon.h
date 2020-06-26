#pragma once
#include "GameObject.h"
#define SIMON_WALKING_SPEED		0.05f 
#define SIMON_JUMP_SPEED_Y		0.5f
#define SIMON_JUMP_DEFLECT_SPEED 0.2f
#define SIMON_GRAVITY			0.002f
#define SIMON_DIE_DEFLECT_SPEED	 0.5f

#define SIMON_STATE_IDLE			0
#define SIMON_STATE_WALKING			1
#define SIMON_STATE_JUMP			3
#define SIMON_STATE_DIE				8
#define SIMON_STATE_HIT				2
#define SIMON_STATE_SIT				4

#define	SIMON_LEVEL_SMALL	2


#define SIMON_BBOX_WIDTH  15
#define SIMON_BBOX_HEIGHT 27

#define SIMON_UNTOUCHABLE_TIME 5000

class Simon : public CGameObject
{
	int level;
	int untouchable;
	DWORD untouchable_start;

	float start_x;
	float start_y;

	bool isDead;
	bool isAttack;
	bool isSit;
	bool isJump;
	bool isMoving;
	bool isExitSit;
	bool isOnCheckStairUp;
	bool isOnCheckStairDown;
	bool isOnStair;
	bool isLeftToRight;
	bool isHurt;
	bool isThrowWeapon = false;
	bool isStand;
	bool isOnGround = false;
	int life;

	static int score;
	static int hearts;
	int currentWeapon;
public:
	Simon(float x = 0.0f, float y = 0.0f);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* colliable_objects = NULL);
	virtual void Render();
	void SetState(int state);
	int GetState() { return state; };
	void SetLevel(int l) { level = 1; }
	void StartUntouchable() { untouchable = 1; untouchable_start = GetTickCount(); }
	void Reset();
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom);

	void AddHeart(int num) { hearts += num; }
	void AddScore(int num) { score += num; }

	int GetScore() { return score; }
	int GetLife() { return life; }
	int GetHeart() { return hearts; }

	void SetCurrentWeapon(int weapon) { currentWeapon = weapon; }
	int GetCurrentWeapon() { return currentWeapon; }
	bool GetIsThrowWeapon() { return isThrowWeapon; }
	void SetIsThrowWeapon(bool isThrowWeapon) { this->isThrowWeapon = isThrowWeapon; }
	bool GetIsStand() { return isStand; }
	void SetIsStand(bool isStand) { this->isStand = isStand; }
	bool CheckStandGround() { return isOnGround; }
};

