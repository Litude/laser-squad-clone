#include "Inventory.hpp"

Inventory::Inventory() { 
	items.resize(MAX_ITEMS);
}

bool Inventory::add(Item& newItem) {
	auto curItem = items.begin();
	while (curItem != items.end()) {
		if (curItem->getMainType() == Type_None) {
			//Found an empty slot
			*curItem = newItem;
			return true;
		}
		++curItem;
	}
	//Inventory is full, item could not be added
	return false;
}
std::list<Item>::iterator Inventory::remove(std::list<Item>::iterator it) {
	return items.erase(it);
}