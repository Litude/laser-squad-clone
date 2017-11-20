#include "GameCharacter.hpp"
#include <iostream>

const int animationFrameTime = 50; // animation frame time in ms
const float MOVE_SPEED = 1.0f; // time it takes in seconds to move from one tile to another

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

void GameCharacter::move(float delta) {
	moveFactor += delta / MOVE_SPEED;
	animationTime += delta;
	if (animationTime * 1000.f > animationFrameTime)
	{
		animationTime = 0.f;
		animation++;
	}
	if (moveFactor >= 1.f) {
		moving = false;
		animation = 0;
		animationTime = 0.f;
		moveFactor = 0.f;
	}
}

sf::Vector2u GameCharacter::getRenderPosition()
{
	sf::Vector2u renderPosition;
	if (moving) {
		// Linear interpolation
		sf::Vector2f interpolatedPosition = sf::Vector2f(currentPosition) * moveFactor + sf::Vector2f(previousPosition) * (1.f - moveFactor);
		renderPosition = sf::Vector2u(interpolatedPosition.x * TILESIZE, interpolatedPosition.y * TILESIZE);
	} else {
		renderPosition = sf::Vector2u(currentPosition.x * TILESIZE, currentPosition.y * TILESIZE);
	}
	return renderPosition;
}