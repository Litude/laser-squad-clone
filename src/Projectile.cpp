#include "Projectile.hpp"

std::shared_ptr<sf::Texture> Projectile::loadTextures() {
	auto proj_textures = std::make_shared<sf::Texture>(sf::Texture());
	if (!proj_textures->loadFromFile("img/projectiles.png")) {
		std::cout << "Could not load 'img/projectiles.png'" << std::endl;
	}
	std::cout << "PROJS LOADED" << std::endl;
	return proj_textures;
}

std::shared_ptr<sf::Texture> Projectile::PROJ_TEXTURES = loadTextures();

bool Projectile::reachedDestination() const {
	return ticks > (distance + delay);
}

sf::Sprite Projectile::drawable() {
	proj.move(m_offset);
	proj.rotate(static_cast<float>(m_spin));
	return proj;
}

bool Projectile::isActive() {
	return ++ticks > delay;
}

std::ostream& operator<<(std::ostream& os, const Projectile& p) {
	os << "Projectile: (" << p.getOrigin().x << ", " << p.getOrigin().y << ") -> (" 
		<< p.getDestination().x << ", " << p.getDestination().y << ")" << std::endl;
	return os;
}

