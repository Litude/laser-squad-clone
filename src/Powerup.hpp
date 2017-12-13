#ifndef LASER_POWERUP_HPP
#define LASER_POWERUP_HPP

#include "Item.hpp"
#include <string>

enum PowerupType {
	Powerup_Health,
	Powerup_LineOfSight,
	Powerup_ActionPoints,
};

class Powerup : public Item {
public:
	Powerup(PowerupType statType, unsigned int increaseAmount, ItemIcon icon, std::string name) : Item(Type_Powerup, icon, name, false, true, 1), m_statType(statType), m_increaseAmount(increaseAmount) {};
	PowerupType getStatType() const { return m_statType; }
	unsigned int getIncreaseAmount() const { return m_increaseAmount; }
private:
	PowerupType	m_statType;
	unsigned int m_increaseAmount;
};

class Armor : public Powerup {
public:
	Armor() : Powerup(Powerup_Health, 5, Icon_Powerup_Armor, "Armor") {};
};


class Sight : public Powerup {
public:
	Sight() : Powerup(Powerup_LineOfSight, 4, Icon_Powerup_Sight, "Bionic Sight") {};
};


class Boots : public Powerup {
public:
	Boots() : Powerup(Powerup_ActionPoints, 5, Icon_Powerup_Boots, "Boots of Running Urgently") {};
};

#endif