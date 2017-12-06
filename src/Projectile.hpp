#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <memory>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include "Item.hpp"

class Projectile : public sf::Drawable {
public:
	//origin and destination in pixels
	Projectile(ItemIcon weaponType, sf::Vector2f origin, sf::Vector2f destination) : m_origin(origin - TILE_CENTER), m_destination(destination - TILE_CENTER) {
		proj.setTexture(*PROJ_TEXTURES);
		sf::Vector2i defSize(32, 32);
		sf::IntRect rect(sf::Vector2i(0, 0), defSize);
		switch (weaponType) {
			case Icon_Weapon_Pistol:
				proj.setTextureRect(rect);
				break;
			case Icon_Weapon_Shotgun:
				rect.left = 64;
				proj.setTextureRect(rect);
				break;
			case Icon_Weapon_Uzi:
				proj.setTextureRect(rect);
				break;
			case Icon_Weapon_Rifle:
				rect.left = 32;
				proj.setTextureRect(rect);
				break;
			case Icon_Weapon_Launcher:
				rect.left = 96;
				proj.setTextureRect(rect);
				break;
			default:
				std::cout << "Unknown weapontype, default texture chosen." << std::endl;
				proj.setTextureRect(rect);
				break;
		}
		proj.setPosition(m_origin);
		sf::Vector2f path = m_destination - m_origin;
		distance = std::sqrt(std::pow(path.y, 2) + std::pow(path.x, 2));
		m_offset = path / distance;
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