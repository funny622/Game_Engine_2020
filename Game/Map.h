#pragma once
#include <string>
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"


#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
using namespace std;
class Map
{
private:
	int ID;

	int tileWidth;
	int tileHeight;

	int translate_y, translate_x;
	int tileRows, tileColumns;
	int mapRows;
	int mapColumns;

	int tileMap[500][500];

	string fileMap;
public:
	Map();
	~Map();
	CGame * game = CGame::GetInstance();
	CTextures* textures = CTextures::GetInstance();

	Map(int _id, int _translate_y, int _translate_x);
	// RECT GetSourceRect(int index);
	void SetMap(string path);
	void LoadMap();
	void DrawMap();
};

