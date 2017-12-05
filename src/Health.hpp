#ifndef LASER_HEALTH_HPP
#define LASER_HEALTH_HPP

#include "Item.hpp"
#include <string>

class Health : public Item {
public:
	Health(unsigned int healingAmount, ItemIcon icon, std::string name) : Item(Type_Health, icon, name), m_HealingAmount(healingAmount) {};
private:
	unsigned int m_HealingAmount;
};

class HealthPackSmall : public Health {
public:
	HealthPackSmall() : Health(25, Icon_Health_Small, "Small Health Pack") {};
};


class HealthPackLarge : public Health {
public:
	HealthPackLarge() : Health(50, Icon_Health_Large, "Large Health Pack") {};
};

#endif