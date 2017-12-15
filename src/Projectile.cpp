#include "Projectile.hpp"

std::shared_ptr<sf::Texture> Projectile::PROJ_TEXTURES = Util::loadTextures("img/projectiles.png");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_PISTOL = Util::loadSound("sound/pistol.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_RIFLE = Util::loadSound("sound/rifle.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_SHOTGUN = Util::loadSound("sound/shotgun.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_LAUNCHER = Util::loadSound("sound/launcher.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_GRENADE = Util::loadSound("sound/grenade.ogg");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_SNIPER = Util::loadSound("sound/sniper.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_PUNCH = Util::loadSound("sound/punch.flac");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_KNIFE = Util::loadSound("sound/throwing_knife.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_SWORD = Util::loadSound("sound/sword.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_LASER = Util::loadSound("sound/laser.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_CROSSBOW = Util::loadSound("sound/raygun.wav");
std::shared_ptr<sf::SoundBuffer> Projectile::SOUND_MINIGUN = Util::loadSound("sound/minigun.wav");

// method hasDeparted must be called as often as proj is drawn for this to work
bool Projectile::reachedDestination() const {
	return ticks > (distance + delay);
}

bool Projectile::soundPlaying() const {
	return sound.getStatus() == sf::SoundSource::Playing;
}

bool Projectile::hasExploded() const {
	return exploded;
}

void Projectile::explode() {
	exploded = true;
}

sf::Sprite Projectile::drawable() {
	if (!soundPlayed) {
		playSound();
	}
	proj.move(m_offset);
	proj.rotate(static_cast<float>(m_spin));
	return proj;
}

bool Projectile::hasDeparted() {
	return ++ticks > delay;
}

void Projectile::playSound() {
	sound.play();
	soundPlayed = true;
}

std::ostream& operator<<(std::ostream& os, const Projectile& p) {
	os << "Projectile: (" << p.getOrigin().x << ", " << p.getOrigin().y << ") -> (" 
		<< p.getDestination().x << ", " << p.getDestination().y << ")" << std::endl;
	return os;
}

