#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <memory>
#include <iostream>
#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "constants.hpp"
#include "Item.hpp"
#include "Util.hpp"
#include "Ammo.hpp"

class Projectile {
public:
	//origin and destination in pixels
	Projectile(AmmoType weaponType, sf::Vector2f origin, sf::Vector2f destination, int delay=0) : atype(weaponType), m_origin(origin), m_destination(destination), delay(delay) {
		proj.setTexture(*PROJ_TEXTURES);
		sf::Vector2i defSize(32, 32);
		sf::IntRect rect(sf::Vector2i(0, 0), defSize);
		m_spin = 0;
		switch (weaponType) {
			case Ammo_None:
				sound.setBuffer(*SOUND_PUNCH);
				proj.setTextureRect(rect);
				break;
			case Ammo_9mm_Bullets:
				sound.setBuffer(*SOUND_PISTOL);
				rect.left = 32;
				proj.setTextureRect(rect);
				break;
			case Ammo_12mm_Bullets:
				sound.setBuffer(*SOUND_RIFLE);
				rect.left = 64;
				proj.setTextureRect(rect);
				break;
			case Ammo_Shotgun_Shells:
				sound.setBuffer(*SOUND_SHOTGUN);
				rect.left = 96;
				proj.setTextureRect(rect);
				break;
			case Ammo_Rockets:
				sound.setBuffer(*SOUND_LAUNCHER);
				rect.left = 128;
				proj.setTextureRect(rect);
				break;
			case Ammo_Knife:
				sound.setBuffer(*SOUND_KNIFE);
				rect.left = 160;
				m_spin = 15;
				proj.setTextureRect(rect);
				break;
			case Ammo_Sword:
				rect.left = 192;
				proj.setTextureRect(rect);
				break;
			case Ammo_Grenades:
				sound.setBuffer(*SOUND_GRENADE);
				rect.left = 224;
				proj.setTextureRect(rect);
				m_spin = 5;
				break;
			case Ammo_Laser_Pack:
				rect.left = 0;
				rect.top = 32;
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
		proj.rotate(Util::vecAngle(path));
	}

	bool hasDeparted();
	bool reachedDestination() const;
	AmmoType getType() const { return atype; }
	sf::Vector2f getOrigin() const { return m_origin; }
	sf::Vector2f getDestination() const { return m_destination; }
	bool soundPlaying() const;
	bool hasExploded() const;
	void explode();
	sf::Sprite drawable();
	operator sf::Sprite() { return drawable(); }

private:
	AmmoType atype;
	sf::Vector2f m_origin;
	sf::Vector2f m_destination;
	// How many ticks to wait before launching projectile
	int delay;
	// How much to degrees spin projectile each tick 
	int m_spin;
	sf::Vector2f m_offset;
	sf::Sprite proj;
	int ticks = 0;
	float distance;
	sf::Sound sound;
	bool soundPlayed = false;
	bool exploded = false;

	void playSound();

	static std::shared_ptr<sf::Texture> PROJ_TEXTURES;

	static std::shared_ptr<sf::SoundBuffer> SOUND_PISTOL;
	static std::shared_ptr<sf::SoundBuffer> SOUND_RIFLE;
	static std::shared_ptr<sf::SoundBuffer> SOUND_SHOTGUN;
	static std::shared_ptr<sf::SoundBuffer> SOUND_LAUNCHER;
	static std::shared_ptr<sf::SoundBuffer> SOUND_GRENADE;
	static std::shared_ptr<sf::SoundBuffer> SOUND_SNIPER;
	static std::shared_ptr<sf::SoundBuffer> SOUND_PUNCH;
	static std::shared_ptr<sf::SoundBuffer> SOUND_KNIFE;
};

std::ostream& operator<<(std::ostream& out, const Projectile& t);

#endif