#include "Map.h"
#include "Game.hpp"
#include <fstream>

using namespace std;

Map::Map() {
}

Map::~Map() {
}

void Map::LoadMap(string path, int sizeX, int sizeY) {
	char tile;
	fstream mapFile;
	mapFile.open(path);

	for (int y = 0; y < sizeY; y++) {
		for (int x = 0; x < sizeX; x++) {
			mapFile.get(tile);
			Game::AddTile(atoi(&tile), x * 16, y * 16);
			mapFile.ignore();
		}
	}

	mapFile.close();
}
