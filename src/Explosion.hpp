#ifndef EXPLOSION_H
#define EXPLOSION_H

#include <memory>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Projectile.hpp"
#include "Util.hpp"

class Explosion {
public:
	Explosion(Projectile& proj) {
		switch(proj.getType()) {
			case Ammo_Grenades:
				sound.setBuffer(*SOUND_EXPL_SMALL);
				explosion.setTexture(*EXPL_SMALL_TEXTURES);
				framesize = sf::Vector2i(96, 96);
				break;
			case Ammo_Rockets:
				sound.setBuffer(*SOUND_EXPL_BIG);
				explosion.setTexture(*EXPL_BIG_TEXTURES);
				framesize = sf::Vector2i(160, 160);
				break;
			case Ammo_Crossbow:
				sound.setBuffer(*SOUND_EXPL_RAY);
				explosion.setTexture(*EXPL_RAY_TEXTURES);
				framesize = sf::Vector2i(96, 96);
				break;
			default:
				explosion.setTexture(*EXPL_SMOKE_TEXTURES);
				framesize = sf::Vector2i(32, 32);
				break;
		}
		rect = sf::IntRect(sf::Vector2i(0, 0), framesize);
		explosion.setTextureRect(rect);

		sf::Vector2f dest = proj.getDestination();
		sf::Vector2f loc(dest.x - (framesize/2).x, dest.y - (framesize/2).y);
		explosion.setPosition(loc);
	}

	bool isActive() const { return active; }
	bool soundPlaying() const;
	sf::Sprite drawable();
	operator sf::Sprite() { return drawable(); }

private:
	sf::Sound sound;
	sf::Sprite explosion;
	sf::Vector2i framesize;
	sf::IntRect rect;
	sf::Clock clock;
	bool active = true;
	int frame = 0;
	
	void changeFrame();
	void setFrame();

	static std::shared_ptr<sf::Texture> EXPL_SMOKE_TEXTURES;
	static std::shared_ptr<sf::Texture> EXPL_SMALL_TEXTURES; 
	static std::shared_ptr<sf::Texture> EXPL_BIG_TEXTURES;
	static std::shared_ptr<sf::Texture> EXPL_RAY_TEXTURES;
	static std::shared_ptr<sf::SoundBuffer> SOUND_EXPL_SMALL;
	static std::shared_ptr<sf::SoundBuffer> SOUND_EXPL_BIG;
	static std::shared_ptr<sf::SoundBuffer> SOUND_EXPL_RAY;
};


#endif