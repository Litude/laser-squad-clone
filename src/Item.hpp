#ifndef LASER_ITEM_HPP
#define LASER_ITEM_HPP

enum ItemMainType {
	Type_None, //used for empty slots in the inventory
	Type_Health,
	Type_Powerup,
	Type_Weapon,
	Type_Ammo,
};

enum ItemSubType {
	//These should be in the same order as in the spritesheet
	SubType_None,
	SubType_Health_Small,
	SubType_Health_Large,
	SubType_Powerup_Armor,
	SubType_Powerup_Accuracy,
	SubType_Weapon_Pistol,
	SubType_Weapon_Shotgun,
	SubType_Weapon_Uzi,
	SubType_Weapon_Rifle,
	SubType_Weapon_Launcher,
	SubType_Ammo_9mm,
	SubType_Ammo_12mm,
	SubType_Ammo_Shells,
	SubType_Ammo_Rockets,
};

//Should only be possible to construct "none"-type items, i.e. empty inventory slots
class Item {
public:
	Item() : m_ItemMainType(Type_None), m_ItemSubType(SubType_None) {};
	Item(ItemMainType mainType, ItemSubType subType) : m_ItemMainType(mainType), m_ItemSubType(subType) {};
	const ItemMainType getMainType() const { return m_ItemMainType;} ;
	const ItemSubType getSubType() const { return m_ItemSubType; };
private:
	ItemMainType m_ItemMainType;
	ItemSubType m_ItemSubType;
};

#endif