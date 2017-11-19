#ifndef LASER_MAP_HPP
#define LASER_MAP_HPP

#include <vector>
#include "Tile.hpp"
#include "GameCharacter.hpp"

class Map {
public:
	Map () : sizeX(0), sizeY(0) { tileMap.resize(1); tileMap.at(0).resize(1); }
	Map (unsigned int sizeX, unsigned int sizeY);

	std::vector<std::vector<Tile>>&		getTileMap() { return tileMap; }
	std::vector<GameCharacter>&			getCharacters() { return characters; }
	Tile&								getTile(unsigned int x, unsigned int y) { return tileMap[y][x]; }
	unsigned int						getSizeX() { return sizeX; }
	unsigned int						getSizeY() { return sizeY; }
	void								characterMoveLeft(unsigned int charIdx);
	void								characterMoveRight(unsigned int charIdx);
	void								characterMoveUp(unsigned int charIdx);
	void								characterMoveDown(unsigned int charIdx);
private:
	unsigned int						sizeX;
	unsigned int						sizeY;
	std::vector<std::vector<Tile>>		tileMap;
	std::vector<GameCharacter>			characters;
};

#endif