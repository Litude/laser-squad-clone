#include <iostream>
#include "gtest/gtest.h"

#include "../Inventory.hpp"
#include "../Weapon.hpp"

TEST(InventoryAmmo, CanFindAmmo) {
	Inventory inventory;
	
	std::shared_ptr<Item> item1 = std::make_shared<Ammo>(AmmoShotgunShells());
	std::shared_ptr<Item> item2 = std::make_shared<Ammo>(AmmoShotgunShells());
	std::shared_ptr<Item> item3 = std::make_shared<Shotgun>(Shotgun());

	auto res = inventory.findAmmo(AmmoType::Ammo_Shotgun_Shells);
	unsigned int availableAmmo = res->getAmount();

	EXPECT_EQ(availableAmmo, 0);

	inventory.add(item1);

	res = inventory.findAmmo(AmmoType::Ammo_Shotgun_Shells);
	availableAmmo = res->getAmount();

	EXPECT_EQ(availableAmmo, item1->getAmount());

	inventory.add(item2);

	res = inventory.findAmmo(AmmoType::Ammo_Shotgun_Shells);
	availableAmmo = res->getAmount();

	EXPECT_EQ(availableAmmo, item1->getAmount() + item2->getAmount());

	inventory.add(item3);

	res = inventory.findAmmo(AmmoType::Ammo_Shotgun_Shells);
	availableAmmo = res->getAmount();

	EXPECT_EQ(availableAmmo, item1->getAmount() + item2->getAmount());
}