#ifndef LASER_GAME_HPP
#define LASER_GAME_HPP

#include <vector>
#include "GameCharacter.hpp"
#include <SFML/System/Vector2.hpp>
#include "Map.hpp"

class Game {
public:
	Game()							{ playerTurnIdx = 1; }
	unsigned int					getCurrentPlayer() { return playerTurnIdx; }
	int								getSelectedCharacter() { return selectedCharacter; }
	Map&							getMap() { return map; }
	void							endTurn();
	bool							addCharacter(sf::Vector2u position, unsigned int team);
	void							setSelectedCharacter(unsigned int idx) { selectedCharacter = idx; }
	void							initializeMap(unsigned int xSize, unsigned int ySize) {map = Map(xSize, ySize);}

private:
	unsigned int					playerTurnIdx;
	unsigned int					selectedCharacter = 1;
	Map								map;
};

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

#endif