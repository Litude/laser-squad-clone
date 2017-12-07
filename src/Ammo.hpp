#ifndef LASER_AMMO_HPP
#define LASER_AMMO_HPP

#include "Item.hpp"

enum AmmoType {
	Ammo_None,
	Ammo_9mm_Bullets,
	Ammo_12mm_Bullets,
	Ammo_Shotgun_Shells,
	Ammo_Rockets,
};

class Ammo : public Item {
public:
	Ammo(ItemIcon icon, std::string name, bool stackable, unsigned int amount, AmmoType ammotype) : Item(Type_Ammo, icon, name, stackable, amount), m_ammoType(ammotype) {};
private:
	AmmoType m_ammoType;
};

class Ammo9mmBullets : public Ammo {
public:
	Ammo9mmBullets() : Ammo(Icon_Ammo_9mm, "9mm Bullets", true, 30, Ammo_9mm_Bullets) {};
};

class AmmoShotgunShells : public Ammo {
public:
	AmmoShotgunShells() : Ammo(Icon_Ammo_Shells, "Shotgun Shells", true, 20, Ammo_Shotgun_Shells) {};
};

#endif