#include "Weapon.hpp"

//WeaponType Weapon::getType() const { return wt; }

unsigned Weapon::getDamage() const { return damage; }

unsigned Weapon::getDelay() const { return shot_delay; }

unsigned Weapon::apCost() const { return ap; }

bool Weapon::canFire() const { return loadedAmmo > 0; }

//return number of shots fired
int Weapon::fire() {
	int shotsFired = 0;
	if (loadedAmmo < shots) {
		shotsFired = loadedAmmo;
		loadedAmmo = 0;
	} else {
		shotsFired = shots;
		loadedAmmo -= shots;
	}
	return shotsFired;
}

//TODO: take an ammo object as parameter to use for reloading
bool Weapon::reload(unsigned ammo) {
	loadedAmmo += ammo;
	return true;
}

// Return true if deviation occurs.
bool Weapon::deviate(sf::Vector2u& target) const {
	if ((rand() % 100 + 1) > accuracy) {
		sf::Vector2i deviated = static_cast<sf::Vector2i>(target);
		int dx = rand() % (deviation*2 + 1) - deviation;
		int dy = rand() % (deviation*2 + 1) - deviation;
		deviated.x += dx;
		deviated.y += dy;
		// Use for debugging
		//std::cout << "Shot deviated by: (" << dx << ", " << dy << ")" << std::endl;
		if (deviated.x < 0) deviated.x = 0;
		if (deviated.y < 0) deviated.y = 0;
		target = static_cast<sf::Vector2u>(deviated);
		return true;
	}
	return false;
}

void Weapon::clampToMaxRange(const sf::Vector2u& origin, sf::Vector2u& target) const {
	auto f_origin = static_cast<sf::Vector2f>(origin); 
	auto f_target = static_cast<sf::Vector2f>(target);
	auto path = f_target - f_origin;
	if (Util::vecLength(path) > (float) range) {
		path = Util::vecUnit(path) * (float) range;
		target = origin + sf::Vector2u(static_cast<unsigned int>(std::round(path.x)), static_cast<unsigned int>(std::round(path.y)));
	}
}
