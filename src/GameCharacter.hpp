#ifndef LASER_CHARACTER_HPP
#define LASER_CHARACTER_HPP

#include <SFML/System/Vector2.hpp>
#include "Inventory.hpp"
#include "Weapon.hpp"
#include "constants.hpp"

#define AP_COST_MOVEMENT 1
#define AP_COST_PICK_ITEM 1
#define AP_COST_DROP_ITEM 1

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
	unsigned int	getMaxActionPoints() const { return maxActionPoints; }
	unsigned int	getHitpoints() const { return health; }
	unsigned int	getTeam() const { return team; }
	sf::Vector2u	getPosition() const { return currentPosition; }
	sf::Vector2u	getRenderPosition() const;
	unsigned int	getDirection() const { return direction; }
	unsigned int	getAnimationFrame() const { return animation; }
	int				getSelectedItem() const { return selectedItem; }
	bool			isDead() const { return (health == 0); }
	bool			isMoving() const { return moving; }
	Weapon&			getEquipped() { return currentItem; }
	bool			moveTo(sf::Vector2i target_dir);
	void			move(int delta_ms);
	int 			shoot();
	void			resetActionPoints() { actionPoints = maxActionPoints; }
	void 			sufferDamage(int dmg);
    unsigned int    getLengthofSight() const {return lengthofSight;}
	bool			addItem(Item obj);
	bool			removeSelectedItem();
	Inventory&		getInventory() { return inventory; }
	void			setSelectedItem(int idx) { selectedItem = idx; }

private:
	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();

	sf::Vector2u currentPosition; // Position on the map in tile coordinates
	sf::Vector2u previousPosition;
	float moveFactor = 0;
	int animationTime = 0;
	direction direction = down;
	unsigned int animation = 0;
	bool moving = false;
	unsigned int maxActionPoints = 20;

	unsigned int actionPoints = maxActionPoints;
	Inventory inventory;
	unsigned int health = 10;
	Weapon currentItem = Weapon();//placeholder
	unsigned int team;
    unsigned int lengthofSight=10;
	int selectedItem = -1;
};

#endif
