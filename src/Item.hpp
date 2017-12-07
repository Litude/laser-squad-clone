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
	Icon_Powerup_Accuracy,
	Icon_Weapon_Pistol,
	Icon_Weapon_Shotgun,
	Icon_Weapon_Uzi,
	Icon_Weapon_Rifle,
	Icon_Weapon_Launcher,
	Icon_Ammo_9mm,
	Icon_Ammo_12mm,
	Icon_Ammo_Shells,
	Icon_Ammo_Rockets,
};

//Should only be possible to construct "none"-type items, i.e. empty inventory slots
class Item {
public:
	Item() : m_itemType(Type_None), m_itemIcon(Icon_None), m_stackable(false), m_amount(1) {};
	virtual ~Item() {};
	Item(ItemType type, ItemIcon icon, std::string name) : m_itemType(type), m_itemIcon(icon), m_itemName(name), m_stackable(false), m_amount(1) {};
	Item(ItemType type, ItemIcon icon, std::string name, bool stackable, unsigned int amount) : m_itemType(type), m_itemIcon(icon), m_itemName(name), m_stackable(stackable), m_amount(amount) {};
	ItemType getType() const { return m_itemType; };
	ItemIcon getIcon() const { return m_itemIcon; };
	std::string getName() const {return m_itemName; };
	bool isStackable() const { return m_stackable; };
	unsigned int getAmount() { return m_amount; };
	void addAmount(unsigned int amount) { m_amount += amount; }
private:
	ItemType m_itemType;
	ItemIcon m_itemIcon;
	std::string m_itemName;
	bool m_stackable;
	unsigned int m_amount;
};

#endif