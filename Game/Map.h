#pragma once
#include <iostream>
#include <fstream>
#include<string>
#include <sstream>
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"


#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
using namespace std;
class Map
{
	CSprites * sprites = CSprites::GetInstance();
	LPCWSTR filePath_data;
	LPCWSTR filePath_texture;

	int ID;

	int tileWidth, tileHeight;
	int tileRows, tileColumns;
	int mapRows, mapColumns;

	vector<vector<LPSPRITE>> tilemap;
public:
	Map(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int mapRows, int mapColumns, int tileRows, int tileColumns, int tileWidth = 32, int tileHeight = 32);
	~Map();
	int GetWidthMap() { return tileColumns * tileWidth; }
	void Load();
	void LoadMap();
	void Draw();
};

