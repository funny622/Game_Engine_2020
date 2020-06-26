#include "Map.h"
#include <stdio.h>
#include <stdlib.h>
#include <fstream>

Map::Map(int ID, LPCWSTR filePath_texture, LPCWSTR filePath_data, int mapRows, int mapColumns, int tileRows, int tileColumns, int tileWidth, int tileHeight)
{
	this->ID = ID;

	this->filePath_texture = filePath_texture;
	this->filePath_data = filePath_data;

	this->mapRows = mapRows;
	this->mapColumns = mapColumns;
	this->tileColumns = tileColumns;
	this->tileRows = tileRows;
	this->tileWidth = tileWidth;
	this->tileHeight = tileHeight;

	LoadMap();
	Load();
}


Map::~Map()
{
}

void Map::Load()
{
	ifstream fs(filePath_data, ios::in);

	if (fs.fail())
	{
		fs.close();
		return;
	}

	string line;

	while (!fs.eof())
	{
		getline(fs, line);
		// Save sprite tile to vector tilemap
		vector<LPSPRITE> spriteline;
		stringstream ss(line);
		int n;

		while (ss >> n)
		{
			int idTile = ID + n;
			spriteline.push_back(sprites->Get(idTile));
		}

		tilemap.push_back(spriteline);
	}

	fs.close();
}

void Map::LoadMap()
{
	CTextures * texture = CTextures::GetInstance();

	texture->Add(ID, filePath_texture, D3DCOLOR_XRGB(255, 0, 255));

	LPDIRECT3DTEXTURE9 texTileMap = texture->Get(ID);


	int id_sprite = 1;
	for (UINT i = 0; i < tileRows; i++)
	{
		for (UINT j = 0; j < tileColumns; j++)
		{
			int id_SPRITE = ID + id_sprite;
			sprites->Add(id_SPRITE, tileWidth * j, tileHeight * i, tileWidth * (j + 1), tileHeight * (i + 1), texTileMap);
			id_sprite++;
		}
	}
}

void Map::Draw()
{
	//Get first and last col belong to camera
	int firstCol = (int)CGame::GetInstance()->GetCamPos().x / tileWidth;
	int lastCol = firstCol + (SCREEN_WIDTH / tileWidth);

	for (UINT i = 0; i < tileRows; i++)
	{
		for (UINT j = firstCol; j <= lastCol; j++)
		{
			float x = tileWidth * (j - firstCol) + CGame::GetInstance()->GetCamPos().x - (int)(CGame::GetInstance()->GetCamPos().x) % tileWidth;
			float y = tileHeight * i + 80;

			tilemap[i][j]->Draw(0, x, y);
		}
	}
}
