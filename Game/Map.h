#pragma once
#include <string>
#include "Utils.h"
#include "Sprites.h"
#include "Textures.h"
#include "Game.h"


#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 320
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
	void _ParseSection_MAP_INFO(string line);
	void _ParseSection_MAP_TILE(string line, int row);
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
	int GetMapWidth();
};

