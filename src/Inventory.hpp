#ifndef LASER_INVENTORY_HPP
#define LASER_INVENTORY_HPP

#include <list>
#include "Item.hpp"

#define MAX_ITEMS 8

class Inventory {
public:
	Inventory();
	bool							add(Item& newItem);
	std::list<Item>::iterator		remove(std::list<Item>::iterator it);
private:
	std::list<Item>					items;
};

#endif