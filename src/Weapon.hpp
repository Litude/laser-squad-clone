#ifndef LASER_WEAPON_HPP
#define LASER_WEAPON_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "Item.hpp"

enum WeaponType { projectile, explosive};

class Weapon : public Item {
public:
	Weapon(ItemSubType TypeOfWeapon, std::string WeaponName, unsigned ap, unsigned loadedAmmo, unsigned damage, unsigned shots, unsigned deviation, WeaponType wt, int accuracy, unsigned range) : Item(Type_Weapon, TypeOfWeapon, WeaponName), ap(ap), loadedAmmo(loadedAmmo), damage(damage), shots(shots), deviation(deviation), wt(wt), accuracy(accuracy), range(range) {};
	bool 		reload(unsigned numberOfAmmo);
	bool		canFire() const;
	int			fire();
	const sf::Vector2u deviate(sf::Vector2u target) const;
	WeaponType getType() const;
	unsigned apCost() const;
private:
	unsigned ap;
	unsigned loadedAmmo;
	unsigned damage;
	unsigned shots;
	//maximum number of tiles the shot deviates 
	unsigned deviation;
	WeaponType wt;

	int accuracy;
	unsigned range;
};

class Hands : public Weapon {
public:
	Hands() : Weapon(SubType_None, "Hands", 1, UINT32_MAX, 3, 1, 1, projectile, 100, 1) {};
};

class Pistol : public Weapon {
public:
	Pistol() : Weapon(SubType_Weapon_Pistol, "Pistol", 5, 3, 5, 2, 1, projectile, 50, 10) {};
};

#endif