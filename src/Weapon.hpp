#ifndef LASER_WEAPON_HPP
#define LASER_WEAPON_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "Item.hpp"

class Weapon : public Item {
public:
	Weapon(ItemIcon icon, std::string name, unsigned ap, unsigned loadedAmmo, unsigned damage, unsigned shots, unsigned deviation, int accuracy, unsigned range) : Item(Type_Weapon, icon, name), ap(ap), loadedAmmo(loadedAmmo), damage(damage), shots(shots), deviation(deviation), accuracy(accuracy), range(range) {};
	bool 		reload(unsigned numberOfAmmo);
	bool		canFire() const;
	int			fire();
	const sf::Vector2u deviate(sf::Vector2u target) const;
	//WeaponType getType() const;
	unsigned getDamage() const;
	int getRange() const { return (int) range; }
	unsigned apCost() const;
	virtual void testInheritance() { std::cout << "Testing failed" << std::endl;  };
private:
	//WeaponType wt
	unsigned ap;
	unsigned loadedAmmo;
	unsigned damage;
	unsigned shots;
	//maximum number of tiles the shot deviates 
	unsigned deviation;

	int accuracy;
	unsigned range;
};

class Hands : public Weapon {
public:
	Hands() : Weapon(Icon_None, "Hands", 1, UINT32_MAX, 3, 1, 1, 100, 1) {};
	virtual void testInheritance() { std::cout << "Testing hands" << std::endl; };
};

class Pistol : public Weapon {
public:
	Pistol() : Weapon(Icon_Weapon_Pistol, "Pistol", 5, 3, 5, 2, 1, 70, 10) {};
	virtual void testInheritance() override { std::cout << "Testing pistol" << std::endl; };
};

class Shotgun : public Weapon {
public:
	Shotgun() : Weapon(Icon_Weapon_Shotgun, "Shotgun", 5, 3, 5, 2, 1, 70, 10) {};
	virtual void testInheritance() override { std::cout << "Testing shotgun" << std::endl; };
};

#endif