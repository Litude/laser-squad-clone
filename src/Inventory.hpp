#ifndef LASER_INVENTORY_HPP
#define LASER_INVENTORY_HPP

#include <vector>
#include "Item.hpp"
#include "constants.hpp"

class Inventory {
public:
	Inventory();
	bool							add(Item& newItem);
	std::vector<Item>::iterator		remove(std::vector<Item>::iterator it);
	Item& operator[](unsigned n);
private:
	std::vector<Item>					items;
};

#endif