#ifndef LASER_CHARACTER_HPP
#define LASER_CHARACTER_HPP

#include <SFML/System/Vector2.hpp>
#include <memory>
#include "Inventory.hpp"
#include "Weapon.hpp"
#include "AnimationManager.hpp"
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

enum animations {
	walk_left,
	walk_right,
	walk_down,
	walk_up,
	die
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
	int				getSelectedItem() const { return selectedItem; }
	bool			isDead() const { return (health == 0); }
	bool			isMoving() const { return moving; }
	std::shared_ptr<Weapon>			getEquipped() { return equippedWeapon; }
	bool			moveTo(sf::Vector2i target_dir);
	void			update(int delta_ms);
	int 			shoot();
	void			resetActionPoints() { actionPoints = maxActionPoints; }
	void 			sufferDamage(int dmg);
    unsigned int    getLengthofSight() const {return lengthofSight;}
	bool			addItem(std::shared_ptr<Item> obj);
	bool			removeSelectedItem();
	Inventory&		getInventory() { return inventory; }
	void			setSelectedItem(int idx) { selectedItem = idx; }
	bool			equipSelected();
	void			setAnimationManager(AnimationManager animationManager) { this->animationManager = animationManager; }
	AnimationManager getAnimationManager() const { return animationManager;  }

private:
	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();

	sf::Vector2u currentPosition; // Position on the map in tile coordinates
	sf::Vector2u previousPosition;

	AnimationManager animationManager;
	float moveFactor = 0;
	direction direction = down;
	bool moving = false;
	unsigned int maxActionPoints = 20;

	unsigned int actionPoints = maxActionPoints;
	Inventory inventory;
	unsigned int health = 5;
	std::shared_ptr<Weapon> equippedWeapon = std::make_shared<Hands>(Hands()); // TODO: This must be freed
	unsigned int team;
    unsigned int lengthofSight=9;
	int selectedItem = -1;
};

#endif
