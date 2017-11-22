#include "GameCharacter.hpp"
#include <iostream>

const int animationFrameTime = 100; // animation frame time in ms
const int moveSpeed = 1000; // time it takes in ms to move from one tile to another

void GameCharacter::moveLeft() {
	previousPosition = currentPosition;
	currentPosition.x -= 1;
	direction = left;
	moving = true;
}

void GameCharacter::moveRight() {
	previousPosition = currentPosition;
	currentPosition.x += 1;
	direction = right;
	moving = true;
}

void GameCharacter::moveUp() {
	previousPosition = currentPosition;
	currentPosition.y -= 1;
	direction = up;
	moving = true;
}

void GameCharacter::moveDown() {
	previousPosition = currentPosition;
	currentPosition.y += 1;
	direction = down;
	moving = true;
}

void GameCharacter::move(int delta_ms) {
	moveFactor += static_cast<float>(delta_ms) / static_cast<float>(moveSpeed);
	animationTime += delta_ms;
	if (animationTime > animationFrameTime)
	{
		// Reset animation time but keep the remainder
		animationTime = animationTime % animationFrameTime;
		animationTime = 0;
		animation++;
	}
	if (moveFactor >= 1.f) {
		moving = false;
		animation = 0;
		animationTime = 0;
		moveFactor = 0.f;
	}
}

sf::Vector2u GameCharacter::getRenderPosition()
{
	sf::Vector2u renderPosition;
	if (moving) {
		// Linear interpolation
		sf::Vector2f interpolatedPosition = sf::Vector2f(currentPosition) * moveFactor + sf::Vector2f(previousPosition) * (1.f - moveFactor);
		renderPosition = sf::Vector2u(static_cast<unsigned int>(interpolatedPosition.x * TILESIZE), static_cast<unsigned int>(interpolatedPosition.y * TILESIZE));
	} else {
		renderPosition = sf::Vector2u(currentPosition.x * TILESIZE, currentPosition.y * TILESIZE);
	}
	return renderPosition;
}