#ifndef LASER_WEAPON_HPP
#define LASER_WEAPON_HPP

#include <string>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include <memory>
#include <SFML/System/Vector2.hpp>
#include "Item.hpp"
#include "Ammo.hpp"
#include "Util.hpp"

class Weapon : public Item {
public:
	Weapon(ItemIcon icon, std::string name, unsigned ap, unsigned loadedAmmo, unsigned damage, unsigned shots, unsigned deviation, int accuracy, unsigned range, AmmoType ammoType, unsigned shot_delay = 100, unsigned area = 0) : Item(Type_Weapon, icon, name), ap(ap), loadedAmmo(loadedAmmo), maxAmmo(loadedAmmo), damage(damage), shots(shots), deviation(deviation), accuracy(accuracy), range(range), ammoType(ammoType), shot_delay(shot_delay), aoe(area) {};
	Weapon(ItemIcon icon, std::string name, bool stackable, bool consumable, unsigned amount, unsigned ap, unsigned loadedAmmo, unsigned damage, unsigned shots, unsigned deviation, int accuracy, unsigned range, AmmoType ammoType, unsigned shot_delay = 100, unsigned area = 0) : Item(Type_Weapon, icon, name, stackable, consumable, amount), ap(ap), loadedAmmo(loadedAmmo), maxAmmo(loadedAmmo), damage(damage), shots(shots), deviation(deviation), accuracy(accuracy), range(range), ammoType(ammoType), shot_delay(shot_delay), aoe(area) {};
	bool 		reload(unsigned numberOfAmmo);
	bool		canFire() const;
	//TODO: combine fire and deviate into single function
	virtual int	fire();
	const sf::Vector2u deviate(sf::Vector2u target) const;
	void clampToMaxRange(const sf::Vector2u& origin, sf::Vector2u& target) const;
	unsigned	getDamage() const;
	unsigned 	getDelay() const;
	int			getRange() const { return (int) range; }
	unsigned	apCost() const;
	unsigned	getLoadedAmmo() const { return loadedAmmo; };
	AmmoType	getAmmoType() const { return ammoType; };
	unsigned	getReloadAmount() const { return maxAmmo - loadedAmmo; };
	unsigned	getArea() const { return aoe; }

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
	// area of effect, 0 for single target
	unsigned aoe;

};

class Hands : public Weapon {
public:
	Hands() : Weapon(Icon_None, "Hands", 1, 1, 3, 1, 1, 100, 1, Ammo_None) {};
	virtual int fire() { return 1; };
};

class Pistol : public Weapon {
public:
	Pistol() : Weapon(Icon_Weapon_Pistol, "Pistol", 5, 6, 7, 2, 1, 60, 10, Ammo_9mm_Bullets) {};
};

class Shotgun : public Weapon {
public:
	Shotgun() : Weapon(Icon_Weapon_Shotgun, "Shotgun", 0, 10000, 5, 5, 1, 0, 10, Ammo_Shotgun_Shells, 0) {};
	//Shotgun() : Weapon(Icon_Weapon_Shotgun, "Shotgun", 5, 3, 5, 2, 1, 70, 10, Ammo_Shotgun_Shells) {};
};

class Uzi : public Weapon {
public:
	Uzi() : Weapon(Icon_Weapon_Uzi, "Uzi", 4, 30, 3, 10, 1, 30, 7, Ammo_9mm_Bullets, 50) {}
};

class Rifle : public Weapon {
public:
	Rifle() : Weapon(Icon_Weapon_Rifle, "Rifle", 5, 12, 6, 3, 1, 70, 12, Ammo_12mm_Bullets) {}
};

class DoubleBarrel : public Weapon {
public:
	DoubleBarrel() : Weapon(Icon_Weapon_DoubleBarrel, "Double Barrel", 8, 16, 5, 8, 2, 0, 8, Ammo_Shotgun_Shells, 0) {}
};

class Sword: public Weapon {
public:
	Sword() : Weapon(Icon_Weapon_Sword, "Sword", 10, 1, 20, 1, 1, 50, 1, Ammo_Sword) {}
	virtual int fire() { return 1; }
};

class Knife : public Weapon {
public:
	Knife() : Weapon(Icon_Weapon_Knife, "Throwing Knife", true, false, 5, 5, 500, 22, 1, 1, 0, 6, Ammo_Knife) {}
	virtual int fire() { if (this->getAmount() >= 1) { this->removeAmount(1); } return 1; };
};

class RocketLauncher : public Weapon {
public:
	RocketLauncher(): Weapon(Icon_Weapon_Launcher, "Rocket Launcher", 8, 2, 25, 1, 2, 20, 12, Ammo_Rockets, 0, 2) {}
};

class Grenade : public Weapon {
public:
	Grenade(): Weapon(Icon_Weapon_Grenade, "Grenade", true, false, 2, 6, 500, 20, 1, 1, 0, 7, Ammo_Grenades, 0, 1) {}
	virtual int fire() { if (this->getAmount() >= 1) { this->removeAmount(1); } return 1;} ;
};

class Laser : public Weapon {
public:
	Laser(): Weapon(Icon_Weapon_Laser, "Heavy Laser", 6, 9, 5, 3, 0, 100, 15, Ammo_Laser_Pack, 50) {}
};

class Sniper : public Weapon {
public:
	Sniper() : Weapon(Icon_Weapon_Sniper, "Sniper", 9, 5, 22, 1, 1, 90, 20, Ammo_50cal_Bullets) {}
};
	

#endif