#include "Game.hpp"

void Game::endTurn() {
	(playerTurnIdx == 1) ? playerTurnIdx = 2 : playerTurnIdx = 1;
	selectedCharacter = -1;
}

bool Game::addCharacter(sf::Vector2u position, unsigned int team) {
	if (team == 1 || team == 2) {
		map.getCharacters().push_back(GameCharacter(position, team));
		return true;
	}
	else {
		return false;
	}
}