#include "GameCharacter.hpp"
#include <iostream>

const int animationFrameTime = 125000; // animation frame time in ms
const int moveSpeed = 500000; // time it takes in ms to move from one tile to another

bool GameCharacter::moveLeft() {
	if (actionPoints > 0) {
		--actionPoints;
		previousPosition = currentPosition;
		currentPosition.x -= 1;
		direction = left;
		moving = true;
		return true;
	}
	return false;
}

bool GameCharacter::moveRight() {
	if (actionPoints > 0) {
		--actionPoints;
		previousPosition = currentPosition;
		currentPosition.x += 1;
		direction = right;
		moving = true;
		return true;
	}
	return false;
}

bool GameCharacter::moveUp() {
	if (actionPoints > 0) {
		--actionPoints;
		previousPosition = currentPosition;
		currentPosition.y -= 1;
		direction = up;
		moving = true;
		return true;
	}
	return false;
}

bool GameCharacter::moveDown() {
	if (actionPoints > 0) {
		--actionPoints;
		previousPosition = currentPosition;
		currentPosition.y += 1;
		direction = down;
		moving = true;
		return true;
	}
	return false;
}

bool GameCharacter::moveTo(sf::Vector2i target_dir) {
	if (isDead()) return false;
	switch(target_dir.x) {
		case 1:
			return moveRight();
			break;
		case -1:
			return moveLeft();
			break;
		default:
			switch(target_dir.y) {
				case 1:
					return moveDown();
					break;
				case -1:
					return moveUp();
					break;
				default:
					break;
			}
			return false;
			break;
	}
}

void GameCharacter::move(int delta_ms) {
	moveFactor += static_cast<float>(delta_ms) / static_cast<float>(moveSpeed);
	animationTime += delta_ms;
	if (animationTime > animationFrameTime)
	{
		// Reset animation time but keep the remainder
		animationTime = animationTime % animationFrameTime;
		animation++;
	}
	if (moveFactor >= 1.f) {
		moving = false;
		animation = 0;
		animationTime = 0;
		moveFactor = 0.f;
	}
}

int GameCharacter::shoot() {
	if (isDead()) return 0;
	if (actionPoints >= currentItem.apCost() && currentItem.canFire()) {
		actionPoints -= currentItem.apCost();
		return currentItem.fire();
	} else {
		return 0;
	}
}

void GameCharacter::sufferDamage(int damage) {
	int armor = 0;//placeholder
	int dmg = (damage - armor > 0 ? damage - armor : 0);
	health = ((int) health - dmg > 0 ? health - dmg : 0);
}

sf::Vector2u GameCharacter::getRenderPosition() const
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
