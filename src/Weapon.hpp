#ifndef LASER_WEAPON_HPP
#define LASER_WEAPON_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <SFML/System/Vector2.hpp>
#include "Item.hpp"
#include "Ammo.hpp"

class Weapon : public Item {
public:
	Weapon(ItemIcon icon, std::string name, unsigned ap, unsigned loadedAmmo, unsigned damage, unsigned shots, unsigned deviation, int accuracy, unsigned range, AmmoType ammoType, unsigned shot_delay=100) : Item(Type_Weapon, icon, name), ap(ap), loadedAmmo(loadedAmmo), maxAmmo(loadedAmmo), damage(damage), shots(shots), deviation(deviation), accuracy(accuracy), range(range), ammoType(ammoType), shot_delay(shot_delay) {};
	bool 		reload(unsigned numberOfAmmo);
	bool		canFire() const;
	int			fire();
	const sf::Vector2u deviate(sf::Vector2u target) const;
	unsigned	getDamage() const;
	unsigned 	getDelay() const;
	int			getRange() const { return (int) range; }
	unsigned	apCost() const;
	unsigned	getLoadedAmmo() const { return loadedAmmo; };
	AmmoType	getAmmoType() const { return ammoType; };
	unsigned	getReloadAmount() const { return maxAmmo - loadedAmmo; };
	virtual void testInheritance() { std::cout << "Testing failed" << std::endl;  };

private:
	//WeaponType wt
	unsigned ap;
	unsigned loadedAmmo;
	unsigned maxAmmo;
	unsigned damage;
	unsigned shots;
	//maximum number of tiles the shot deviates 
	unsigned deviation;
	int accuracy;
	unsigned range;
	AmmoType ammoType;
	// delay between shots, used for rendering only
	unsigned shot_delay;
};

class Hands : public Weapon {
public:
	Hands() : Weapon(Icon_None, "Hands", 1, UINT32_MAX, 3, 1, 1, 100, 1, Ammo_None) {};
	virtual void testInheritance() { std::cout << "Testing hands" << std::endl; };
};

class Pistol : public Weapon {
public:
	Pistol() : Weapon(Icon_Weapon_Pistol, "Pistol", 5, 3, 5, 2, 1, 70, 10, Ammo_9mm_Bullets) {};
	virtual void testInheritance() override { std::cout << "Testing pistol" << std::endl; };
};

class Shotgun : public Weapon {
public:
	Shotgun() : Weapon(Icon_Weapon_Shotgun, "Shotgun", 0, 10000, 5, 5, 2, 0, 10, Ammo_Shotgun_Shells, 0) {};
	//Shotgun() : Weapon(Icon_Weapon_Shotgun, "Shotgun", 5, 3, 5, 2, 1, 70, 10, Ammo_Shotgun_Shells) {};
	virtual void testInheritance() override { std::cout << "Testing shotgun" << std::endl; };
};

class Uzi : public Weapon {
public:
	Uzi() : Weapon(Icon_Weapon_Uzi, "Uzi", 0, 500, 3, 10, 1, 50, 10, Ammo_9mm_Bullets, 50) {}
	virtual void testInheritance() override {std::cout << "Testing uzi" << std::endl; }
};

#endif