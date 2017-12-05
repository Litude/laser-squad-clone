#include "Inventory.hpp"
#include <iostream>

Inventory::Inventory() { 
	for (unsigned int i = 0; i < MAX_ITEMS; i++) {
		items.push_back(std::make_shared<Item>(Item()));
	}
	//items.resize(MAX_ITEMS);
}

bool Inventory::add(std::shared_ptr<Item>& newItem) {
	if (newItem->getType() == Type_None) return false; //do not add "empty" items

	auto curItem = items.begin();
	while (curItem != items.end()) {
		if ((*curItem)->getType() == Type_None) {
			//Found an empty slot
			*curItem = newItem;
			return true;
		}
		++curItem;
	}
	//Inventory is full, item could not be added
	return false;
}
std::vector<std::shared_ptr<Item>>::iterator Inventory::remove(std::vector<std::shared_ptr<Item>>::iterator it) {
	return items.erase(it);
}

std::shared_ptr<Item>& Inventory::operator[](unsigned n) {
	return items[n];
}