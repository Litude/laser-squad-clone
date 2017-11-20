#ifndef LASER_CHARACTER_HPP
#define LASER_CHARACTER_HPP

#include <SFML/System/Vector2.hpp>
#include "Inventory.hpp"
#include "constants.hpp"

enum direction {
	left,
	right,
	down,
	up
};

class GameCharacter {
public:
	GameCharacter (sf::Vector2u position, unsigned int team) : currentPosition(position), previousPosition(position), team(team) { }
	unsigned int	getActionPoints() { return actionPoints; }
	unsigned int	getHitpoints() { return health; }
	unsigned int	getTeam() { return team; }
	sf::Vector2u	getPosition() { return currentPosition; }
	sf::Vector2u	getRenderPosition();
	unsigned int	getDirection() { return direction; }
	unsigned int	getAnimationFrame() { return animation; }
	bool			isDead() { return dead; }
	bool			isMoving() { return moving; }
	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();
	void			move(float delta);

private:
	sf::Vector2u currentPosition; // Position on the map in tile coordinates
	sf::Vector2u previousPosition;
	float moveFactor = 0;
	float animationTime = 0;
	direction direction = down;
	unsigned int animation = 0;
	bool moving = false;

	unsigned int actionPoints;
	Inventory inventory;
	bool dead;
	unsigned int health;
	unsigned int currentItem;
	unsigned int team;
};

#endif