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
	unsigned int	getActionPoints() const { return actionPoints; }
	unsigned int	getHitpoints() const { return health; }
	unsigned int	getTeam() const { return team; }
	sf::Vector2u	getPosition() const { return currentPosition; }
	sf::Vector2u	getRenderPosition() const;
	unsigned int	getDirection() const { return direction; }
	unsigned int	getAnimationFrame() const { return animation; }
	bool			isDead() const { return dead; }
	bool			isMoving() const { return moving; }
	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();
	void			moveTo(sf::Vector2i target_dir);
	void			move(int delta_ms);

private:
	sf::Vector2u currentPosition; // Position on the map in tile coordinates
	sf::Vector2u previousPosition;
	float moveFactor = 0;
	int animationTime = 0;
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