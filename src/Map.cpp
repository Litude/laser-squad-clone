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
	if (characters[charIdx].getPosition().x > 0) {
		if (getTile((characters[charIdx].getPosition().x) - 1, characters[charIdx].getPosition().y).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x - 1, characters[charIdx].getPosition().y)) {
				return;
			}
		}
		characters[charIdx].moveLeft();
	}
}

void Map::characterMoveRight(unsigned int charIdx) {
	if (characters[charIdx].getPosition().x < (tileMap.at(0).size() - 1)) {
		if (getTile((characters[charIdx].getPosition().x) + 1, characters[charIdx].getPosition().y).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x + 1, characters[charIdx].getPosition().y)) {
				return;
			}
		}
		characters[charIdx].moveRight();
	}
}

void Map::characterMoveUp(unsigned int charIdx) {
	if (characters[charIdx].getPosition().y > 0) {
		if (getTile(characters[charIdx].getPosition().x, (characters[charIdx].getPosition().y) - 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x, characters[charIdx].getPosition().y - 1)) {
				return;
			}
		}
		characters[charIdx].moveUp();
	}
}

void Map::characterMoveDown(unsigned int charIdx) {
	if (characters[charIdx].getPosition().y < (tileMap.at(0).size() - 1) * TILESIZE) {
		if (getTile(characters[charIdx].getPosition().x, (characters[charIdx].getPosition().y) + 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x, characters[charIdx].getPosition().y + 1)) {
				return;
			}
		}
		characters[charIdx].moveDown();
	}
}