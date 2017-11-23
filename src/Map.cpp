#include "Map.hpp"
#include <iostream>

Map::Map(unsigned int newsizeX, unsigned int newsizeY) {
	sizeX = newsizeX;
	sizeY = newsizeY;

	tileMap.resize(newsizeY);
	for (unsigned int yi = 0; yi < sizeY; ++yi) {
		std::vector<Tile> column;
		column.resize(newsizeX, Tile());
		tileMap[yi] = column;
	}
}