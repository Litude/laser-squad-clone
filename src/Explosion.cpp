#include "Explosion.hpp"
#define FRAME_DURATION 25
#define NUMBER_OF_FRAMES 40

std::shared_ptr<sf::Texture> Explosion::loadTextures(std::string filepath) {
	auto expl_textures = std::make_shared<sf::Texture>();
	if (!expl_textures->loadFromFile(filepath)) {
		std::cout << "Could not load " << filepath << std::endl;
	}
	std::cout << "EXPLOSIONS LOADED" << std::endl;
	return expl_textures;
}

std::shared_ptr<sf::Texture> Explosion::EXPL_SMOKE_TEXTURES = loadTextures("img/smoke.png");
std::shared_ptr<sf::Texture> Explosion::EXPL_SMALL_TEXTURES = loadTextures("img/explosion_small.png");
std::shared_ptr<sf::Texture> Explosion::EXPL_BIG_TEXTURES = loadTextures("img/explosion_big.png");

sf::Sprite Explosion::drawable() {
	int delta = clock.getElapsedTime().asMilliseconds();
	if (delta > FRAME_DURATION) {
		changeFrame();
		clock.restart();
	}
	return explosion;
}

void Explosion::changeFrame() {
	if (frame == NUMBER_OF_FRAMES) {
		active = false;
	} else {
		++frame;
		setFrame();
	}
}

void Explosion::setFrame() {
	// Check to ensure no out of bounds before calling
	if (frame % 8 == 0) {
		rect.left = 0;
		rect.top += framesize.y;
	} else {
		rect.left += framesize.x;
	}
	explosion.setTextureRect(rect);
}