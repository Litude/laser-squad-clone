#include <SFML/Graphics.hpp>
#include <vector>

#include "animationManager.hpp"

AnimationManager::AnimationManager()
{
	this->t = 0;
	this->currentAnim = 0;
}

AnimationManager::AnimationManager(const sf::IntRect& frameSize)
{
	this->frameSize = frameSize;

	sf::IntRect rect = this->frameSize;
	this->bounds = rect;

	this->t = 0;
	this->currentAnim = 0;
}

void AnimationManager::update(const unsigned int dt)
{
	if (currentAnim >= static_cast<int>(this->animations.size()) || currentAnim < 0) return;

	int duration = this->animations[currentAnim].duration;

	// If the time has progressed to a new animation frame, change it
	if (int((t + dt) / duration) > int(t / duration))
	{
		// Calculate the frame number
		unsigned int frame = unsigned int((t + dt) / duration);

		// If reached end and not looping, return
		if (frame >= this->animations[currentAnim].getLength() && !this->animations[currentAnim].loop) {
			return;
		}

		// Adjust for looping
		frame %= this->animations[currentAnim].getLength();

		// Update the animation bounds to current frame
		sf::IntRect rect = this->frameSize;
		rect.left = rect.width * frame;
		rect.top = rect.height * this->animations[currentAnim].offset;
		this->bounds = rect;
	}

	// Increment the elapsed time
	this->t += dt;
	// Adjust for looping
	if (this->t > duration * this->animations[currentAnim].getLength())
	{
		this->t = 0;
	}
}

void AnimationManager::addAnim(Animation& anim)
{
	this->animations.push_back(anim);
}

void AnimationManager::changeAnim(unsigned int animID)
{
	// Check that animation is different from currently active one and that new animation exists
	if (this->currentAnim == animID || animID >= this->animations.size() ||
		animID < 0) return;

	// Set the current animation
	this->currentAnim = animID;
	// Update the animation bounds
	sf::IntRect rect = this->frameSize;
	rect.top = rect.height * this->animations[animID].offset;
	this->bounds = rect;
	this->t = 0;
}