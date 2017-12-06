#include "Projectile.hpp"

std::shared_ptr<sf::Texture> Projectile::loadTextures() {
	auto proj_textures = std::make_shared<sf::Texture>(sf::Texture());
	if (!proj_textures->loadFromFile("img/projectiles.png")) {
		std::cout << "Could not load 'img/projectiles.png'" << std::endl;
	}
	return proj_textures;
}

std::shared_ptr<sf::Texture> Projectile::PROJ_TEXTURES = loadTextures();

bool Projectile::isActive() const {
	return active;
}

sf::Sprite Projectile::drawable() {
	if (active) {
		proj.move(m_offset);
	}
	if (++ticks > distance) active = false;
	return proj;
}

std::ostream& operator<<(std::ostream& os, const Projectile& p) {
	os << "Projectile: (" << p.getOrigin().x << ", " << p.getOrigin().y << ") -> (" 
		<< p.getDestination().x << ", " << p.getDestination().y << ")" << std::endl;
	return os;
}

