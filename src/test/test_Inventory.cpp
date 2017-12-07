#include <iostream>
#include "gtest/gtest.h"

#include "../Inventory.hpp"
#include "../Weapon.hpp"
#include "../Health.hpp"

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

	//Since ammo of the same type stacks, the total ammo amount should be under item1
	EXPECT_EQ(availableAmmo, item1->getAmount());

	inventory.add(item3);

	res = inventory.findAmmo(AmmoType::Ammo_Shotgun_Shells);
	availableAmmo = res->getAmount();

	EXPECT_EQ(availableAmmo, item1->getAmount());
}

TEST(InventoryAmmo, StackAmounts) {
	Inventory inventory;
	AmmoShotgunShells shellInstance;
	Ammo9mmBullets bulletInstance;
	unsigned int shellRefAmount = shellInstance.getAmount();
	unsigned int bulletRefAmount = bulletInstance.getAmount();

	std::shared_ptr<Item> item1 = std::make_shared<Ammo>(AmmoShotgunShells());
	std::shared_ptr<Item> item2 = std::make_shared<Ammo>(AmmoShotgunShells());
	std::shared_ptr<Item> item3 = std::make_shared<Ammo>(Ammo9mmBullets());
	std::shared_ptr<Item> item4 = std::make_shared<Ammo>(Ammo9mmBullets());

	inventory.add(item1); //All shotgun shells should stack to this item

	EXPECT_EQ(item1->getAmount(), shellRefAmount);

	inventory.add(item2);

	EXPECT_EQ(item1->getAmount(), shellRefAmount * 2);

	inventory.add(item3); //All 9mm bullet should stack to this item

	EXPECT_EQ(item1->getAmount(), shellRefAmount * 2);
	EXPECT_EQ(item3->getAmount(), bulletRefAmount);

	inventory.add(item4);

	EXPECT_EQ(item1->getAmount(), shellRefAmount * 2);
	EXPECT_EQ(item3->getAmount(), bulletRefAmount * 2);

}

TEST(InventoryCapacity, Adding) {
	Inventory inventory;

	//Add stuff until max capacity (8 pcs)
	std::shared_ptr<Item> item1 = std::make_shared<HealthPackSmall>(HealthPackSmall());
	std::shared_ptr<Item> item2 = std::make_shared<HealthPackSmall>(HealthPackSmall());
	std::shared_ptr<Item> item3 = std::make_shared<Pistol>(Pistol());
	std::shared_ptr<Item> item4 = std::make_shared<HealthPackSmall>(HealthPackSmall());
	std::shared_ptr<Item> item5 = std::make_shared<AmmoShotgunShells>(AmmoShotgunShells());
	std::shared_ptr<Item> item6 = std::make_shared<HealthPackLarge>(HealthPackLarge());
	std::shared_ptr<Item> item7 = std::make_shared<Ammo9mmBullets>(Ammo9mmBullets());
	std::shared_ptr<Item> item8 = std::make_shared<Shotgun>(Shotgun());

	EXPECT_EQ(true, inventory.add(item1));
	EXPECT_EQ(true, inventory.add(item2));
	EXPECT_EQ(true, inventory.add(item3));
	EXPECT_EQ(true, inventory.add(item4));
	EXPECT_EQ(true, inventory.add(item5));
	EXPECT_EQ(true, inventory.add(item6));
	EXPECT_EQ(true, inventory.add(item7));
	EXPECT_EQ(true, inventory.add(item8));

	//Try adding even more stuff that isn't stackable, should not get added
	std::shared_ptr<Item> item9 = std::make_shared<Pistol>(Pistol());

	EXPECT_EQ(false, inventory.add(item9));

	//Try stackable stuff which should still get added
	std::shared_ptr<Item> item10 = std::make_shared<Ammo9mmBullets>(Ammo9mmBullets());
	std::shared_ptr<Item> item11 = std::make_shared<AmmoShotgunShells>(AmmoShotgunShells());

	EXPECT_EQ(true, inventory.add(item10));
	EXPECT_EQ(true, inventory.add(item11));

}