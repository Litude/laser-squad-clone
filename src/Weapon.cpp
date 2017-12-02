#include "Weapon.hpp"

WeaponType Weapon::getType() const { return wt; }

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

const sf::Vector2u Weapon::deviate(sf::Vector2u target) const {
	sf::Vector2i deviated = static_cast<sf::Vector2i>(target);
	if ((rand() % 100 + 1) > accuracy) {
		int dx = rand() % 3 - 1;
		int dy = rand() % 3 - 1;
		deviated.x += dx;
		deviated.y += dy;
		std::cout << "Shot deviated by: (" << dx << ", " << dy << ")" << std::endl;
	}
	if (deviated.x < 0) deviated.x = 0;
	if (deviated.y < 0) deviated.y = 0;
	return static_cast<sf::Vector2u>(deviated);
}

