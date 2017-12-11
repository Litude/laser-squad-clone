#ifndef LASER_CHARACTER_HPP
#define LASER_CHARACTER_HPP

#include <SFML/System/Vector2.hpp>
#include <memory>
#include <algorithm>
#include "Inventory.hpp"
#include "Weapon.hpp"
#include "Health.hpp"
#include "AnimationManager.hpp"
#include "constants.hpp"

#define AP_COST_MOVEMENT 1
#define AP_COST_PICK_ITEM 1
#define AP_COST_DROP_ITEM 1
#define AP_COST_USE 1
#define AP_COST_RELOAD 1

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

enum statuscode {
	nothing,
	not_enough_ap,
	item_weapon_equipped,
	item_weapon_unequipped,
	item_healed,
	item_max_health,
	item_unusable,
	item_none_selected,
	shoot_reload,
	shoot_no_ammo,
	shoot_success,
	inventory_full,
};

class GameCharacter {
public:
	GameCharacter (sf::Vector2u position, unsigned int team) : currentPosition(position), previousPosition(position), team(team) { }
	unsigned int	getActionPoints() const { return actionPoints; }
	unsigned int	getMaxActionPoints() const { return maxActionPoints; }
	unsigned int	getHitpoints() const { return health; }
	unsigned int	getMaxHitpoints() const { return maxHealth; }
	unsigned int	getTeam() const { return team; }
	sf::Vector2u	getPosition() const { return currentPosition; }
	sf::Vector2u	getRenderPosition() const;
	unsigned int	getDirection() const { return char_dir; }
	int				getSelectedItemIndex() const { return selectedItemIdx; }
	int				getSelectedWeaponIndex() const { return selectedWeaponIdx; }
	void			setHitpoints(unsigned hp);
	bool			isDead() const { return (health == 0); }
	bool			isMoving() const { return moving; }
	std::shared_ptr<Weapon>			getEquipped() { return equippedWeapon; }
	bool			moveTo(sf::Vector2i target_dir);
	void			update(int delta_ms);
	statuscode 		shoot(int &numberOfShots);
	void			resetActionPoints() { actionPoints = maxActionPoints; }
	bool 			sufferDamage(int dmg);
    unsigned int    getLengthofSight() const {return lengthofSight;}
	statuscode		addItem(std::shared_ptr<Item> obj);
	bool			removeSelectedItem();
	Inventory&		getInventory() { return inventory; }
	void			setSelectedItemIndex(int idx) { selectedItemIdx = idx; }
	void			setSelectedWeaponIndex(int idx) {selectedWeaponIdx = idx; }
	statuscode		useSelected();
	void			unequipCharacter();
	void			setAnimationManager(AnimationManager animationManager) { this->animationManager = animationManager; }
	AnimationManager getAnimationManager() const { return animationManager;  }
	unsigned int	getAmmoAmount(AmmoType ammotype, unsigned int neededAmount=0);
	statuscode		reloadWeapon();
	bool			shouldBeRemoved() { return remove; };

private:
	void			moveLeft();
	void			moveRight();
	void			moveUp();
	void			moveDown();

	sf::Vector2u currentPosition; // Position on the map in tile coordinates
	sf::Vector2u previousPosition;

	AnimationManager animationManager;
	float moveFactor = 0;
	direction char_dir = down;
	bool moving = false;
	unsigned int maxActionPoints = 20;

	unsigned int actionPoints = maxActionPoints;
	Inventory inventory;
	unsigned int health = 10;
	unsigned int maxHealth = 10;
	std::shared_ptr<Weapon> equippedWeapon = std::make_shared<Hands>();
	unsigned int team;
    unsigned int lengthofSight = 9;
	int selectedItemIdx = -1;
	int selectedWeaponIdx = -1;
	bool remove = false;
};

#endif
