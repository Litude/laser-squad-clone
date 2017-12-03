#ifndef LASER_HEALTH_HPP
#define LASER_HEALTH_HPP

#include "Item.hpp"
#include <string>

class Health : public Item {
public:
	Health(unsigned int HealingAmount, ItemSubType HealthSubType, std::string ItemName) : Item(Type_Health, HealthSubType, ItemName), m_HealingAmount(HealingAmount) {};
private:
	unsigned int m_HealingAmount;
};

class HealthPackSmall : public Health {
public:
	HealthPackSmall() : Health(25, SubType_Health_Small, "Small Health Pack") {};
};


class HealthPackLarge : public Health {
public:
	HealthPackLarge() : Health(50, SubType_Health_Large, "Large Health Pack") {};
};

#endif