#include "Game.hpp"

void Game::endTurn() {
	(playerTurnIdx == 1) ? playerTurnIdx = 2 : playerTurnIdx = 1;
	selectedCharacter = characters.end();
	for (auto &character: characters) {
		character.resetActionPoints();
	}
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

bool Game::characterMove(std::vector<GameCharacter>::iterator it, sf::Vector2i direction) {
	sf::Vector2i target_pos = (sf::Vector2i) it->getPosition() + direction;
	if (!getGrid()(target_pos).isSolid() && std::all_of(characters.begin(), characters.end(), 
			[target_pos](GameCharacter gc){ return (sf::Vector2i) gc.getPosition() != target_pos; })) {
		it->moveTo(direction);
		return true;
	} else {
		return false;
	}
}

bool Game::characterMoveLeft(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(-1, 0);
	return characterMove(it, dir);
}

bool Game::characterMoveRight(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(1, 0);
	return characterMove(it, dir);
}

bool Game::characterMoveUp(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(0, -1);
	return characterMove(it, dir);
}

bool Game::characterMoveDown(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(0, 1);
	return characterMove(it, dir);
}