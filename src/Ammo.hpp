#ifndef LASER_AMMO_HPP
#define LASER_AMMO_HPP

#include "Item.hpp"

enum AmmoType {
	Ammo_None,
	Ammo_9mm_Bullets,
	Ammo_12mm_Bullets,
	Ammo_Shotgun_Shells,
	Ammo_Rockets,
	Ammo_Knife,
	Ammo_Sword,
	Ammo_Grenades,
	Ammo_Laser_Pack,
	Ammo_50cal_Bullets
};

class Ammo : public Item {
public:
	Ammo(ItemIcon icon, std::string name, bool stackable, unsigned int amount, AmmoType ammotype) : Item(Type_Ammo, icon, name, stackable, false, amount), m_ammoType(ammotype) {};
	AmmoType getAmmoType() const { return m_ammoType; }
private:
	AmmoType m_ammoType;
};

class Ammo9mmBullets : public Ammo {
public:
	Ammo9mmBullets() : Ammo(Icon_Ammo_9mm, "9mm Bullets", true, 30, Ammo_9mm_Bullets) {};
};

class Ammo12mmBullets : public Ammo {
public:
	Ammo12mmBullets() : Ammo(Icon_Ammo_12mm, "12mm Bullets", true, 30, Ammo_12mm_Bullets) {};
};

class AmmoRockets : public Ammo {
public:
	AmmoRockets() : Ammo(Icon_Ammo_Rockets, "Rockets", true, 3, Ammo_Rockets) {};
};

class AmmoShotgunShells : public Ammo {
public:
	AmmoShotgunShells() : Ammo(Icon_Ammo_Shells, "Shotgun Shells", true, 20, Ammo_Shotgun_Shells) {};
};

class AmmoLaserPack : public Ammo {
public:
	AmmoLaserPack() : Ammo(Icon_Ammo_LaserPack, "Laser Pack", true, 36, Ammo_Laser_Pack) {};
};

class Ammo50calBullets : public Ammo {
public:
	Ammo50calBullets() : Ammo(Icon_Ammo_50cal, "50cal Bullets", true, 5, Ammo_50cal_Bullets) {};
};

#endif