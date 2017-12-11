#ifndef LASER_INVENTORY_HPP
#define LASER_INVENTORY_HPP

#include <vector>
#include <memory>
#include "Item.hpp"
#include "constants.hpp"
#include "Ammo.hpp"

class Inventory {
public:
	typedef std::vector<std::shared_ptr<Item>>::const_iterator const_iterator;
	typedef std::vector<std::shared_ptr<Item>>::iterator iterator;

	Inventory();
	bool											add(std::shared_ptr<Item>& newItem);
	void											remove(unsigned int itemIdx);
	void											remove(std::shared_ptr<Item> itemPtr);
	std::shared_ptr<Item>&							operator[](unsigned n);
	std::shared_ptr<Item>							findAmmo(AmmoType ammo);
	const_iterator									begin() const { return items.begin(); }
	const_iterator									end() const { return items.end(); }
	iterator										begin() { return items.begin(); }
	iterator										end() { return items.end(); }
private:
	std::vector<std::shared_ptr<Item>>				items;
};

#endif