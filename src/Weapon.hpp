#ifndef LASER_WEAPON_HPP
#define LASER_WEAPON_HPP

class Weapon {
public:
	void		Fire();
	std::string getType();
private:
	unsigned int ammo;
	unsigned int damage;
	double accuracy;
};

#endif