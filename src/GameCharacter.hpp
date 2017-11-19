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
	GameCharacter (sf::Vector2u position, unsigned int team) : currentPosition(position), actualPosition(position), team(team) { }
	unsigned int	getActionPoints() { return actionPoints; }
	unsigned int	getHitpoints() { return health; }
	unsigned int	getTeam() { return team; }
	unsigned int	getPositionX() { return currentPosition.x; }
	unsigned int	getPositionY() { return currentPosition.y; }
	sf::Vector2u	getCollisionPosition() { return actualPosition; }
	unsigned int	getDirection() { return direction; }
	unsigned int	getAnimationFrame() { return animation; }
	bool			isDead() { return dead; }
	bool			isMoving() { return moving; }
	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();
	void			move();

private:
	sf::Vector2u currentPosition; //Position used for drawing the character
	sf::Vector2u actualPosition; //Position used for collision detection
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