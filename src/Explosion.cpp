#include "Explosion.hpp"
#define FRAME_DURATION 25
#define NUMBER_OF_FRAMES 40

std::shared_ptr<sf::Texture> Explosion::EXPL_SMOKE_TEXTURES = Util::loadTextures("img/smoke.png");
std::shared_ptr<sf::Texture> Explosion::EXPL_SMALL_TEXTURES = Util::loadTextures("img/explosion_small.png");
std::shared_ptr<sf::Texture> Explosion::EXPL_BIG_TEXTURES = Util::loadTextures("img/explosion_big.png");
std::shared_ptr<sf::Texture> Explosion::EXPL_RAY_TEXTURES = Util::loadTextures("img/explosion_ray.png");
std::shared_ptr<sf::SoundBuffer> Explosion::SOUND_EXPL_SMALL = Util::loadSound("sound/explosion_small.ogg");
std::shared_ptr<sf::SoundBuffer> Explosion::SOUND_EXPL_BIG = Util::loadSound("sound/explosion_big.ogg");
std::shared_ptr<sf::SoundBuffer> Explosion::SOUND_EXPL_RAY = Util::loadSound("sound/explosion_ray.ogg");

sf::Sprite Explosion::drawable() {
	if (active && !soundPlaying()) {
		sound.play();
	}
	int delta = clock.getElapsedTime().asMilliseconds();
	if (delta > FRAME_DURATION) {
		changeFrame();
		clock.restart();
	}
	return explosion;
}

bool Explosion::soundPlaying() const {
	return sound.getStatus() == sf::SoundSource::Playing;
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