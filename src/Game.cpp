#include "Game.hpp"

void Game::endTurn() {
	(playerTurnIdx == 1) ? playerTurnIdx = 2 : playerTurnIdx = 1;
	selectedCharacter = -1;
}

bool Game::addCharacter(sf::Vector2u position, unsigned int team) {
	if (team == 1 || team == 2) {
		characters.push_back(GameCharacter(position, team));
		return true;
	}
	else {
		return false;
	}
}

void Game::characterMoveLeft(unsigned int charIdx) {
	if (characters[charIdx].getPosition().x > 0) {
		if (getMap().getTile((characters[charIdx].getPosition().x) - 1, characters[charIdx].getPosition().y).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x - 1, characters[charIdx].getPosition().y)) {
				return;
			}
		}
		characters[charIdx].moveLeft();
	}
}

void Game::characterMoveRight(unsigned int charIdx) {
	if (characters[charIdx].getPosition().x < (getMap().getTileMap().at(0).size() - 1)) {
		if (getMap().getTile((characters[charIdx].getPosition().x) + 1, characters[charIdx].getPosition().y).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x + 1, characters[charIdx].getPosition().y)) {
				return;
			}
		}
		characters[charIdx].moveRight();
	}
}

void Game::characterMoveUp(unsigned int charIdx) {
	if (characters[charIdx].getPosition().y > 0) {
		if (getMap().getTile(characters[charIdx].getPosition().x, (characters[charIdx].getPosition().y) - 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x, characters[charIdx].getPosition().y - 1)) {
				return;
			}
		}
		characters[charIdx].moveUp();
	}
}

void Game::characterMoveDown(unsigned int charIdx) {
	if (characters[charIdx].getPosition().y < (getMap().getTileMap().at(0).size() - 1) * TILESIZE) {
		if (getMap().getTile(characters[charIdx].getPosition().x, (characters[charIdx].getPosition().y) + 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(characters[charIdx].getPosition().x, characters[charIdx].getPosition().y + 1)) {
				return;
			}
		}
		characters[charIdx].moveDown();
	}
}