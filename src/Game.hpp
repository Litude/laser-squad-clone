#ifndef LASER_GAME_HPP
#define LASER_GAME_HPP

#include <vector>
#include "GameCharacter.hpp"
#include <SFML/System/Vector2.hpp>
#include "Grid.hpp"

class Game {
public:
	Game()									{ playerTurnIdx = 1; }
	unsigned int							getCurrentPlayer() { return playerTurnIdx; }
	std::vector<GameCharacter>::iterator	getSelectedCharacter() { return selectedCharacter; }
	Grid&									getGrid() { return grid; }
	void									endTurn();
	bool									addCharacter(sf::Vector2u position, unsigned int team);
	void									setSelectedCharacter(std::vector<GameCharacter>::iterator it) { selectedCharacter = it; }
	void									initializeGrid(unsigned int xSize, unsigned int ySize) {grid = Grid(xSize*ySize, xSize);}
	std::vector<GameCharacter>&				getCharacters() { return characters; }
	void									characterMoveLeft(std::vector<GameCharacter>::iterator it);
	void									characterMoveRight(std::vector<GameCharacter>::iterator it);
	void									characterMoveUp(std::vector<GameCharacter>::iterator it);
	void									characterMoveDown(std::vector<GameCharacter>::iterator it);

private:
	unsigned int							playerTurnIdx;
	std::vector<GameCharacter>::iterator	selectedCharacter;
	Grid									grid;
	std::vector<GameCharacter>				characters;
};

#endif