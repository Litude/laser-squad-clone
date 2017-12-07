#include "Item.hpp"

enum AmmoType {
	Ammo_9mm_Bullets,
	Ammo_12mm_Bullets,
	Ammo_Shotgun_Shells,
	Ammo_Rockets,
};

class Ammo : public Item {
public:
	Ammo(ItemIcon icon, std::string name, bool stackable, unsigned int amount) : Item(Type_Ammo, icon, name, stackable, amount) {};
private:
	AmmoType m_ammoType;
};

class Ammo9mmBullets : public Ammo {
public:
	Ammo9mmBullets() : Ammo(Icon_Ammo_9mm, "9mm Bullets", true, 30) {};
};
