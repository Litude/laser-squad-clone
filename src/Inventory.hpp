#ifndef LASER_INVENTORY_HPP
#define LASER_INVENTORY_HPP

#include <vector>
#include <memory>
#include "Item.hpp"
#include "constants.hpp"
#include "Ammo.hpp"

class Inventory {
public:
	Inventory();
	bool											add(std::shared_ptr<Item>& newItem);
	std::vector<std::shared_ptr<Item>>::iterator	remove(std::vector<std::shared_ptr<Item>>::iterator it);
	std::shared_ptr<Item>&							operator[](unsigned n);
	std::shared_ptr<Item>							findAmmo(AmmoType ammo);
private:
	std::vector<std::shared_ptr<Item>>				items;
};

#endif