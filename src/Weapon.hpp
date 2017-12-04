#ifndef LASER_WEAPON_HPP
#define LASER_WEAPON_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <SFML/System/Vector2.hpp>

enum WeaponType { projectile, explosive };

class Weapon {
public:
	bool 		reload(unsigned numberOfAmmo);
	bool		canFire() const;
	int			fire();
	const sf::Vector2u deviate(sf::Vector2u target) const;
	WeaponType getType() const;
	unsigned getDamage() const;
	unsigned apCost() const;
private:
	unsigned ap = 5;
	unsigned loadedAmmo = 3;
	unsigned damage = 5;
	unsigned shots = 2;
	//maximum number of tiles the shot deviates 
	unsigned deviation = 1;
	WeaponType wt = projectile;

	int accuracy = 70;
};

#endif