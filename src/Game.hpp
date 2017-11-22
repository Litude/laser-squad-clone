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
	std::vector<GameCharacter>&		getCharacters() { return characters; }
	void							characterMoveLeft(unsigned int charIdx);
	void							characterMoveRight(unsigned int charIdx);
	void							characterMoveUp(unsigned int charIdx);
	void							characterMoveDown(unsigned int charIdx);

private:
	unsigned int					playerTurnIdx;
	unsigned int					selectedCharacter = 1;
	Map								map;
	std::vector<GameCharacter>			characters;
};

#endif