#ifndef LASER_ANIMATIONMANAGER_HPP
#define LASER_ANIMATIONMANAGER_HPP

#include "Animation.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class AnimationManager
{
public:
	AnimationManager();
	AnimationManager(const sf::IntRect& frameSize);
	void addAnim(Animation& anim);
	void update(const unsigned int dt);
	void changeAnim(const unsigned int animNum);
	const bool isFinished() const;
	const sf::IntRect& getFrame() const { return bounds; }
private:
	std::vector<Animation> animations;
	unsigned int t;
	sf::IntRect bounds;
	sf::IntRect frameSize;

	int currentAnim;
};

#endif