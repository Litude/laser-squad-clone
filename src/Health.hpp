#ifndef LASER_HEALTH_HPP
#define LASER_HEALTH_HPP

#include "Item.hpp"
#include <string>

class Health : public Item {
public:
	Health(unsigned int healingAmount, ItemIcon icon, std::string name) : Item(Type_Health, icon, name), m_healingAmount(healingAmount) {};
	unsigned int getHealingAmount() const { return m_healingAmount; }
private:
	unsigned int m_healingAmount;
};

class HealthPackSmall : public Health {
public:
	HealthPackSmall() : Health(2, Icon_Health_Small, "Small Health Pack") {};
};


class HealthPackLarge : public Health {
public:
	HealthPackLarge() : Health(5, Icon_Health_Large, "Large Health Pack") {};
};

#endif