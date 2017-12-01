#ifndef LASER_GAME_HPP
#define LASER_GAME_HPP

#include <vector>
#include <algorithm>
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
	bool									characterMoveLeft(std::vector<GameCharacter>::iterator it);
	bool									characterMoveRight(std::vector<GameCharacter>::iterator it);
	bool									characterMoveUp(std::vector<GameCharacter>::iterator it);
	bool									characterMoveDown(std::vector<GameCharacter>::iterator it);
	bool                                    characterMove(std::vector<GameCharacter>::iterator it, sf::Vector2i direction);
    std::vector<sf::Vector2u>               seenCoordinates(std::vector<GameCharacter>::iterator it);
    bool                                    lineofSight(int x1,int y1,int x2,int y2);
private:
	unsigned int							playerTurnIdx;
	std::vector<GameCharacter>::iterator	selectedCharacter;
	Grid									grid;
	std::vector<GameCharacter>				characters;
};

#endif
