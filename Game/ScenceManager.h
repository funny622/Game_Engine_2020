#pragma once

#include <d3dx9.h>
#include "KeyEventHandler.h"

class CScenceManager
{
protected:
	CKeyEventHandler * key_handler;
	int id;
	LPCWSTR sceneFilePath;

public:
	CScenceManager(int id, LPCWSTR filePath);

	CKeyEventHandler * GetKeyEventHandler() { return key_handler; }
	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Update(DWORD dt) = 0;
	virtual void Render() = 0;
};
typedef CScenceManager * LPSCENE;


class CScenceKeyHandler : public CKeyEventHandler
{
protected:
	CScenceManager * scence;

public:
	virtual void KeyState(BYTE *states) = 0;
	virtual void OnKeyDown(int KeyCode) = 0;
	virtual void OnKeyUp(int KeyCode) = 0;
	CScenceKeyHandler(CScenceManager *s) :CKeyEventHandler() { scence = s; }
};