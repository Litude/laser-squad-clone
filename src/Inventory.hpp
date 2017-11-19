#include <list>
#include "Weapon.hpp"

#define MAX_ITEMS 8

class Inventory {
public:
	bool							add(Weapon& newWeapon);
	std::list<Weapon>::iterator		remove(std::list<Weapon>::iterator it);
private:
	std::list<Weapon>					items;
};