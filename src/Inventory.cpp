#include "Inventory.hpp"
#include <iostream>

Inventory::Inventory() { 
	items.resize(MAX_ITEMS);
}

bool Inventory::add(Item& newItem) {
	if (newItem.getMainType() == Type_None) return false; //do not add "empty" items

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
std::vector<Item>::iterator Inventory::remove(std::vector<Item>::iterator it) {
	return items.erase(it);
}

Item& Inventory::operator[](unsigned n) {
	return items[n];
}