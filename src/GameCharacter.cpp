#include "GameCharacter.hpp"
#include <iostream>

const int animationFrameTime = 125000; // animation frame time in ms
const int moveSpeed = 500000; // time it takes in ms to move from one tile to another

void GameCharacter::moveLeft() {
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.x -= 1;
	direction = left;
	moving = true;
}

void GameCharacter::moveRight() {
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.x += 1;
	direction = right;
	moving = true;
}

void GameCharacter::moveUp() {
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.y -= 1;
	direction = up;
	moving = true;
}

void GameCharacter::moveDown() {
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.y += 1;
	direction = down;
	moving = true;
}

bool GameCharacter::moveTo(sf::Vector2i target_dir) {
	if (isDead()) return false;

	if (actionPoints >= AP_COST_MOVEMENT) {
		switch (target_dir.x) {
		case 1:
			moveRight();
			return true;
		case -1:
			moveLeft();
			return true;
		default:
			switch (target_dir.y) {
			case 1:
				moveDown();
				return true;
			case -1:
				moveUp();
				return true;
			default:
				return false;
			}
			return false;
		}
	}
	return false;
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

bool GameCharacter::addItem(std::shared_ptr<Item> obj)
{
	if (actionPoints >= AP_COST_PICK_ITEM) {
		if (inventory.add(obj)) {
			actionPoints -= AP_COST_PICK_ITEM;
			return true;
		}
	}
	return false;
}

bool GameCharacter::removeSelectedItem()
{
	if (actionPoints >= AP_COST_DROP_ITEM) {
		inventory[selectedItem] = std::make_shared<Item>(Item());
		selectedItem = -1;
		actionPoints -= AP_COST_DROP_ITEM;
		return true;
	}
	return false;
}