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

	if (newItem->isStackable()) {
		auto emptySlot = items.end();
		while (curItem != items.end()) {
			//Search for both stackable slots and empty slots simultaneously
			if ((*curItem)->getType() == Type_None && emptySlot == items.end()) emptySlot = curItem;
			if ((*curItem)->getIcon() == newItem->getIcon()) {
				(*curItem)->addAmount(newItem->getAmount());
				return true;
			}
			++curItem;
		}
		if (emptySlot != items.end()) {
			*emptySlot = newItem;
			return true;
		}
	}

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

std::shared_ptr<Item> Inventory::findAmmo(AmmoType ammo) {
	auto curItem = items.begin();
	while (curItem != items.end()) {
		if ((*curItem)->getType() == Type_Ammo) {
			if ((std::dynamic_pointer_cast<Ammo>(*curItem)->getAmmoType()) == ammo) {
				return *curItem;
			};
		}
		++curItem;
	}
	//Did not find any ammo, return a dummy item
	return std::make_shared<Ammo>(Ammo(Icon_None, "None", false, 0, Ammo_None));
}