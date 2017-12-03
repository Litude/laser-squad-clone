#ifndef LASER_HEALTH_HPP
#define LASER_HEALTH_HPP

#include "Item.hpp"

class Health : public Item {
public:
	Health(unsigned int HealingAmount, ItemSubType HealthSubType) : Item(Type_Health, HealthSubType), m_HealingAmount(HealingAmount) {};
private:
	unsigned int m_HealingAmount;
};

class Health_Small : public Health {
public:
	Health_Small() : Health(25, SubType_Health_Small) {};
};


class Health_Large : public Health {
public:
	Health_Large() : Health(50, SubType_Health_Large) {};
};

#endif