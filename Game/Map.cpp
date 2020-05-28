#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#define MAX_MAP_LINE 1024
#define MAP_SECTION_UNKNOWN 0
#define MAP_SECTION_INFO 1
#define MAP_SECTION_TILE_MAP 2
Map::Map()
{
}


Map::~Map()
{
}

Map::Map(int _id, int _translate_y, int _translate_x)
{
	ID = _id;
	translate_y = _translate_y;
	translate_x = _translate_x;
}
void Map::LoadMap()
{
	ifstream f;
	f.open(fileMap);

	char str[MAX_MAP_LINE];
	int section = MAP_SECTION_UNKNOWN;
	int row = 0;
	while (f.getline(str, MAX_MAP_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[INFO]") {
			section = MAP_SECTION_INFO;
			continue;
		}
		if(line == "[TILE_MAP]")
		{
			section = MAP_SECTION_TILE_MAP;
			continue;
		}

		switch (section)
		{
		case MAP_SECTION_INFO: 
			_ParseSection_MAP_INFO(line); 
			break;
		case MAP_SECTION_TILE_MAP: 
			_ParseSection_MAP_TILE(line, row); 
			row++;
			break;
		}
	}
}

void Map::_ParseSection_MAP_INFO(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines
	mapRows = atoi(tokens[0].c_str());
	mapColumns = atoi(tokens[1].c_str());
	tileWidth = atoi(tokens[2].c_str());
	tileHeight = atoi(tokens[3].c_str());
	tileColumns = atoi(tokens[4].c_str());
	tileRows = atoi(tokens[5].c_str());
}

void Map::_ParseSection_MAP_TILE(string line, int row)
{
	vector<string> tokens = split(line);

	if (tokens.size() < mapColumns) return; // skip invalid lines
	for (int j = 0; j < mapColumns; j++)
		tileMap[row][j] = atoi(tokens[j].c_str());
}

void Map::DrawMap()
{
	float cam_x = CGame::GetInstance()->GetCamPos().x;
	float cam_y = CGame::GetInstance()->GetCamPos().y;

	float remain_x = fmod(cam_x, tileWidth);
	float remain_y = fmod(cam_y, tileHeight);

	// draw tiles within the viewport only
	int colCamLeft = cam_x / tileWidth;
	int rowCamTop = cam_y / tileHeight;

	int colCamRight = colCamLeft + SCREEN_WIDTH / tileWidth + SCREEN_WIDTH / 2;
	int rowCamBottom = rowCamTop + SCREEN_HEIGHT / tileHeight;

	for (int j = colCamLeft; j <= colCamRight; j++)
	{
		for (int i = rowCamTop; i < rowCamBottom; i++)
		{
			float pos_x = (j - colCamLeft) * tileWidth - remain_x + translate_x;
			float pos_y = (i - rowCamTop) * tileHeight - remain_y + translate_y;

			RECT rectTile;
			int index = tileMap[i][j];
			rectTile.left = (index % tileColumns) * tileWidth;
			rectTile.top = (index / tileColumns) * tileHeight;
			rectTile.right = rectTile.left + tileWidth;
			rectTile.bottom = rectTile.top + tileHeight;

			CGame::GetInstance()->Draw(pos_x, pos_y, 1, CTextures::GetInstance()->Get(1000), rectTile.left, rectTile.top, rectTile.right, rectTile.bottom);
		}
	}
}

void Map::SetMap(string path)
{
	fileMap = path;
}

int Map::GetMapWidth()
{
	return mapColumns * tileWidth;
}
