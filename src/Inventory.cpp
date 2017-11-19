#include "Inventory.hpp"

Inventory::Inventory() { 
	items.resize(MAX_ITEMS);
}

bool Inventory::add(Weapon& newWeapon) {
	auto curItem = items.begin();
	while (curItem != items.end()) {
		if (curItem->getType() == "") {
			//Found an empty slot
			*curItem = newWeapon;
			return true;
		}
		++curItem;
	}
	//Inventory is full, item could not be added
	return false;
}
std::list<Weapon>::iterator Inventory::remove(std::list<Weapon>::iterator it) {
	return items.erase(it);
}