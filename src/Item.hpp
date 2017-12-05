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
	Item() : m_ItemType(Type_None), m_ItemIcon(Icon_None) {};
	virtual ~Item() {};
	Item(ItemType type, ItemIcon icon, std::string name) : m_ItemType(type), m_ItemIcon(icon), m_ItemName(name) {};
	ItemType getType() const { return m_ItemType;} ;
	ItemIcon getIcon() const { return m_ItemIcon; };
	std::string getName() const {return m_ItemName; };
private:
	ItemType m_ItemType;
	ItemIcon m_ItemIcon;
	std::string m_ItemName;
};

#endif