#ifndef LASER_GAME_HPP
#define LASER_GAME_HPP

#include <vector>
#include "GameCharacter.hpp"
#include <SFML/System/Vector2.hpp>
#include "Map.hpp"

class Game {
public:
	Game()									{ playerTurnIdx = 1; }
	unsigned int							getCurrentPlayer() { return playerTurnIdx; }
	std::vector<GameCharacter>::iterator	getSelectedCharacter() { return selectedCharacter; }
	Map&									getMap() { return map; }
	void									endTurn();
	bool									addCharacter(sf::Vector2u position, unsigned int team);
	void									setSelectedCharacter(std::vector<GameCharacter>::iterator it) { selectedCharacter = it; }
	void									initializeMap(unsigned int xSize, unsigned int ySize) {map = Map(xSize, ySize);}
	std::vector<GameCharacter>&				getCharacters() { return characters; }
	void									characterMoveLeft(std::vector<GameCharacter>::iterator it);
	void									characterMoveRight(std::vector<GameCharacter>::iterator it);
	void									characterMoveUp(std::vector<GameCharacter>::iterator it);
	void									characterMoveDown(std::vector<GameCharacter>::iterator it);

private:
	unsigned int							playerTurnIdx;
	std::vector<GameCharacter>::iterator	selectedCharacter;
	Map										map;
	std::vector<GameCharacter>				characters;
};

#endif