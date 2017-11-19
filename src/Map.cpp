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

void Map::characterMoveLeft(unsigned int charIdx) {
	if (characters[charIdx].getPositionX() > 0) {
		if (getTile((characters[charIdx].getPositionX() / TILESIZE) - 1, characters[charIdx].getPositionY() / TILESIZE).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getCollisionPosition() == sf::Vector2u(characters[charIdx].getCollisionPosition().x - TILESIZE, characters[charIdx].getCollisionPosition().y)) {
				return;
			}
		}
		characters[charIdx].moveLeft();
	}
}

void Map::characterMoveRight(unsigned int charIdx) {
	if (characters[charIdx].getPositionX() < (tileMap.at(0).size() - 1) * TILESIZE) {
		if (getTile((characters[charIdx].getPositionX() / TILESIZE) + 1, characters[charIdx].getPositionY() / TILESIZE).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getCollisionPosition() == sf::Vector2u(characters[charIdx].getCollisionPosition().x + TILESIZE, characters[charIdx].getCollisionPosition().y)) {
				return;
			}
		}
		characters[charIdx].moveRight();
	}
}

void Map::characterMoveUp(unsigned int charIdx) {
	if (characters[charIdx].getPositionY() > 0) {
		if (getTile(characters[charIdx].getPositionX() / TILESIZE, (characters[charIdx].getPositionY() / TILESIZE) - 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getCollisionPosition() == sf::Vector2u(characters[charIdx].getCollisionPosition().x, characters[charIdx].getCollisionPosition().y - TILESIZE)) {
				return;
			}
		}
		characters[charIdx].moveUp();
	}
}

void Map::characterMoveDown(unsigned int charIdx) {
	if (characters[charIdx].getPositionY() < (tileMap.at(0).size() - 1) * TILESIZE) {
		if (getTile(characters[charIdx].getPositionX() / TILESIZE, (characters[charIdx].getPositionY() / TILESIZE) + 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getCollisionPosition() == sf::Vector2u(characters[charIdx].getCollisionPosition().x, characters[charIdx].getCollisionPosition().y + TILESIZE)) {
				return;
			}
		}
		characters[charIdx].moveDown();
	}
}