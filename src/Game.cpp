#include "Game.hpp"

void Game::endTurn() {
	(playerTurnIdx == 1) ? playerTurnIdx = 2 : playerTurnIdx = 1;
	selectedCharacter = characters.end();
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

void Game::characterMoveLeft(std::vector<GameCharacter>::iterator it) {
	if (it->getPosition().x > 0) {
		if (getMap().getTile((it->getPosition().x) - 1, it->getPosition().y).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(it->getPosition().x - 1, it->getPosition().y)) {
				return;
			}
		}
		it->moveLeft();
	}
}

void Game::characterMoveRight(std::vector<GameCharacter>::iterator it) {
	if (it->getPosition().x < (getMap().getTileMap().at(0).size() - 1)) {
		if (getMap().getTile((it->getPosition().x) + 1, it->getPosition().y).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(it->getPosition().x + 1, it->getPosition().y)) {
				return;
			}
		}
		it->moveRight();
	}
}

void Game::characterMoveUp(std::vector<GameCharacter>::iterator it) {
	if (it->getPosition().y > 0) {
		if (getMap().getTile(it->getPosition().x, (it->getPosition().y) - 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(it->getPosition().x, it->getPosition().y - 1)) {
				return;
			}
		}
		it->moveUp();
	}
}

void Game::characterMoveDown(std::vector<GameCharacter>::iterator it) {
	if (it->getPosition().y < (getMap().getTileMap().at(0).size() - 1) * TILESIZE) {
		if (getMap().getTile(it->getPosition().x, (it->getPosition().y) + 1).isSolid() == true) return; //check for solid block

		for (auto character : characters) {
			if (character.getPosition() == sf::Vector2u(it->getPosition().x, it->getPosition().y + 1)) {
				return;
			}
		}
		it->moveDown();
	}
}