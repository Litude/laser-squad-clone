#ifndef LASER_ANIMATIONMANAGER_HPP
#define LASER_ANIMATIONMANAGER_HPP

#include "Animation.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class AnimationManager
{
public:
	void addAnim(Animation& anim);
	void update(const unsigned int dt);
	void changeAnim(unsigned int animNum);
	bool isFinished();
	AnimationManager();
	AnimationManager(const sf::IntRect& frameSize);
	sf::IntRect getFrame() { return bounds; }
private:
	std::vector<Animation> animations;
	unsigned int t;
	sf::IntRect bounds;
	sf::IntRect frameSize;

	int currentAnim;
};

#endif