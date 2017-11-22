#ifndef LASER_MAP_HPP
#define LASER_MAP_HPP

#include <vector>
#include "Tile.hpp"

class Map {
public:
	Map () : sizeX(0), sizeY(0) { tileMap.resize(1); tileMap.at(0).resize(1); }
	Map (unsigned int sizeX, unsigned int sizeY);

	std::vector<std::vector<Tile>>&		getTileMap() { return tileMap; }
	Tile&								getTile(unsigned int x, unsigned int y) { return tileMap[y][x]; }
	unsigned int						getSizeX() { return sizeX; }
	unsigned int						getSizeY() { return sizeY; }
private:
	unsigned int						sizeX;
	unsigned int						sizeY;
	std::vector<std::vector<Tile>>		tileMap;
};

#endif