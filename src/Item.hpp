#ifndef LASER_ITEM_HPP
#define LASER_ITEM_HPP

#include <string>

enum ItemType {
	Type_None, //used for empty slots in the inventory
	Type_Health,
	Type_Powerup,
	Type_Weapon,
	Type_Ammo,
};

enum ItemIcon {
	//These should be in the same order as in the spritesheet
	Icon_None,
	Icon_Health_Small,
	Icon_Health_Large,
	Icon_Powerup_Armor,
	Icon_Powerup_Sight,
	Icon_Weapon_Pistol,
	Icon_Weapon_Shotgun,
	Icon_Weapon_Uzi,
	Icon_Weapon_Rifle,
	Icon_Weapon_Launcher,
	Icon_Ammo_9mm,
	Icon_Ammo_12mm,
	Icon_Ammo_Shells,
	Icon_Ammo_Rockets,
	Icon_Powerup_Boots,
	Icon_Weapon_Knife,
	Icon_Weapon_Sword,
	Icon_Weapon_Grenade,
	Icon_Weapon_DoubleBarrel,
	Icon_Weapon_Laser,
	Icon_Ammo_LaserPack,
	Icon_Weapon_Sniper,
	Icon_Ammo_50cal,
	Icon_Weapon_Crossbow,
};

//Should only be possible to construct "none"-type items, i.e. empty inventory slots
class Item {
public:
	Item() : m_itemType(Type_None), m_itemIcon(Icon_None), m_stackable(false), m_amount(1) {};
	virtual ~Item() {};
	Item(const Item& copyFrom) = default;
	Item& operator=(const Item& copyFrom) = default;
	Item(Item &&) = default;
	Item& operator=(Item &&) = default;

	Item(ItemType type, ItemIcon icon, std::string name) : m_itemType(type), m_itemIcon(icon), m_itemName(name), m_consumable(false), m_stackable(false), m_amount(1) {};
	Item(ItemType type, ItemIcon icon, std::string name, bool stackable, bool consumable, unsigned int amount) : m_itemType(type), m_itemIcon(icon), m_itemName(name), m_consumable(consumable), m_stackable(stackable), m_amount(amount) {};
	ItemType getType() const { return m_itemType; };
	ItemIcon getIcon() const { return m_itemIcon; };
	std::string getName() const {return m_itemName; };
	bool isStackable() const { return m_stackable; };
	bool isConsumeable() const { return m_consumable; };
	unsigned int getAmount() { return m_amount; };
	void addAmount(unsigned int amount) { m_amount += amount; }
	void removeAmount(unsigned int amount) { m_amount -= amount; }
	void setAmount(unsigned int amount) { m_amount = amount;}
private:
	ItemType m_itemType;
	ItemIcon m_itemIcon;
	std::string m_itemName;
	bool m_consumable; //Items that should be "consumed" when used from inventory
	bool m_stackable;
	unsigned int m_amount;
};

#endif