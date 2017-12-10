#ifndef LASER_GAME_HPP
#define LASER_GAME_HPP

#include <vector>
#include <algorithm>
#include <iterator>
#include <utility>
#include "GameCharacter.hpp"
#include <SFML/System/Vector2.hpp>
#include "Grid.hpp"
#include "Util.hpp"


class Game {
public:
	typedef std::vector<GameCharacter>::iterator gc_iterator;
	typedef std::vector<sf::Vector2u>::const_iterator coord_iterator; 
	Game()									{ playerTurnIdx = 1; }
	unsigned int							getCurrentPlayer() { return playerTurnIdx; }
	std::vector<GameCharacter>::iterator	getSelectedCharacter() { return selectedCharacter; }
	const Grid&								getGrid() const { return grid; }
	Grid&									getGrid() { return grid; }
	void									endTurn();
	bool									addCharacter(sf::Vector2u position, unsigned int team);
    bool                                    removeCharacter(sf::Vector2u position);
	void									setSelectedCharacter(gc_iterator it) { selectedCharacter = it; }
	void									initializeGrid(unsigned int xSize, unsigned int ySize) {grid = Grid(xSize*ySize, xSize);}
	std::vector<GameCharacter>&				getCharacters() { return characters; }
	bool                                    characterMoveLeft(gc_iterator it);
	bool                                    characterMoveRight(gc_iterator it);
	bool                                    characterMoveUp(gc_iterator it);
	bool                                    characterMoveDown(gc_iterator it);
	bool                                    characterMove(gc_iterator it, sf::Vector2i direction);
	const std::vector<sf::Vector2u>         characterShoot(gc_iterator gc, sf::Vector2u target);
	bool									characterPickUpItem(gc_iterator it);
	bool									characterDropItem(gc_iterator it);
	const sf::Vector2u                      traceFromCharacter(gc_iterator gc, sf::Vector2u target, bool ignoreCharacters=false);
	const sf::Vector2u                      getEndTile(coord_iterator coord_begin, coord_iterator coord_end, bool ignoreCharacters, int maxRange=12);
    std::vector<sf::Vector2u>               seenCoordinates(gc_iterator it);
    bool                                    lineofSight(int x1,int y1,int x2,int y2);
	bool									calculateLineofSight() {return recalculateLOS;}
	void									lineofSightCalculated() { recalculateLOS = false; }
private:
	unsigned int							playerTurnIdx;
	bool									recalculateLOS = true;
	std::vector<GameCharacter>::iterator	selectedCharacter;
	Grid									grid;
	std::vector<GameCharacter>				characters;
};

#endif
