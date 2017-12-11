#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <memory>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "Item.hpp"
#include "Util.hpp"
#include "Ammo.hpp"

class Projectile {
public:
	//origin and destination in pixels
	Projectile(AmmoType weaponType, sf::Vector2f origin, sf::Vector2f destination) : m_origin(origin), m_destination(destination) {
		proj.setTexture(*PROJ_TEXTURES);
		sf::Vector2i defSize(32, 32);
		sf::IntRect rect(sf::Vector2i(0, 0), defSize);
		switch (weaponType) {
			case Ammo_None:
				proj.setTextureRect(rect);
				break;
			case Ammo_9mm_Bullets:
				rect.left = 32;
				proj.setTextureRect(rect);
				break;
			case Ammo_12mm_Bullets:
				rect.left = 64;
				proj.setTextureRect(rect);
				break;
			case Ammo_Shotgun_Shells:
				rect.left = 96;
				proj.setTextureRect(rect);
				break;
			case Ammo_Rockets:
				rect.left = 128;
				proj.setTextureRect(rect);
				break;
			default:
				std::cout << "Unknown weapontype, default texture chosen." << std::endl;
				proj.setTextureRect(rect);
				break;
		}
		proj.setOrigin(TILE_CENTER);

		sf::Vector2f path = m_destination - m_origin;
		distance = std::sqrt(std::pow(path.y, 2) + std::pow(path.x, 2));
		m_offset = path / distance;
		proj.setPosition(m_origin);
		std::cout << "ANGLE: " << Util::vecAngle(path) << std::endl;
		proj.rotate(Util::vecAngle(path));
	}

	bool isActive() const;
	void draw(sf::RenderTarget&, sf::RenderStates) const { }
	const sf::Vector2f& getOrigin() const { return m_origin; }
	const sf::Vector2f& getDestination() const { return m_destination; }

	operator sf::Sprite() { return drawable(); }
	sf::Sprite drawable();

private:
	sf::Vector2f m_origin;
	sf::Vector2f m_destination;
	sf::Vector2f m_offset;
	sf::Sprite proj;
	bool active = true;
	int ticks = 0;
	float distance;

	static std::shared_ptr<sf::Texture> loadTextures();
	static std::shared_ptr<sf::Texture> PROJ_TEXTURES;	
};

std::ostream& operator<<(std::ostream& out, const Projectile& t);

#endif