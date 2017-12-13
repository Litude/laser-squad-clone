#include "GameCharacter.hpp"
#include <iostream>


const int moveSpeed = 500000; // time it takes in ms to move from one tile to another

GameCharacter::GameCharacter(sf::Vector2u position, unsigned int team) : 
	currentPosition(position), previousPosition(position), team(team)
{
	// Setup animations
	Animation animation_walk_left(9, 0, 8, 62000);
	Animation animation_walk_right(11, 0, 8, 62000);
	Animation animation_walk_down(10, 0, 8, 62000);
	Animation animation_walk_up(8, 0, 8, 62000);
	Animation animation_die(20, 0, 5, 125000, false);
	animationManager  =AnimationManager(sf::IntRect(0, 0, 32, 32));
	animationManager.addAnim(animation_walk_left);
	animationManager.addAnim(animation_walk_right);
	animationManager.addAnim(animation_walk_down);
	animationManager.addAnim(animation_walk_up);
	animationManager.addAnim(animation_die);
	animationManager.changeAnim(animations::walk_down); // Initial animation
}

void GameCharacter::setHitpoints(unsigned hp) {
	if (hp > maxHealth) {
		maxHealth = hp;
		health = maxHealth;
	} else {
		health = (hp == 0 ? 1 : hp);
	}
}

void GameCharacter::moveLeft() {
	animationManager.changeAnim(animations::walk_left);
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.x -= 1;
	char_dir = left;
	moving = true;
}

void GameCharacter::moveRight() {
	animationManager.changeAnim(animations::walk_right);
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.x += 1;
	char_dir = right;
	moving = true;
}

void GameCharacter::moveUp() {
	animationManager.changeAnim(animations::walk_up);
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.y -= 1;
	char_dir = up;
	moving = true;
}

void GameCharacter::moveDown() {
	animationManager.changeAnim(animations::walk_down);
	actionPoints -= AP_COST_MOVEMENT;
	previousPosition = currentPosition;
	currentPosition.y += 1;
	char_dir = down;
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
		}
	}
	return false;
}

void GameCharacter::update(int delta_ms) {
	if (isDead()) {
		animationManager.update(delta_ms);
		if (animationManager.isFinished()) remove = true;
	}
	if (moving) {
		animationManager.update(delta_ms);
		moveFactor += static_cast<float>(delta_ms) / static_cast<float>(moveSpeed);
		if (moveFactor >= 1.f) {
			moving = false;
			moveFactor = 0.f;
		}
	}
}

statuscode GameCharacter::shoot(int &numberOfShots) {
	if (isDead()) return nothing;
	if (!getEquipped()->canFire()) {
		return reloadWeapon();
	}

	if (actionPoints >= getEquipped()->apCost() && getEquipped()->canFire()) {
		actionPoints -= getEquipped()->apCost();
		numberOfShots = getEquipped()->fire();
		return shoot_success;
	} else {
		return not_enough_ap;
	}
}

bool GameCharacter::sufferDamage(int damage) {
	//Returns true if the target character was killed, false otherwise

	int armor = 0;//placeholder
	int dmg = (damage - armor > 0 ? damage - armor : 0);
	health = ((int) health - dmg > 0 ? health - dmg : 0);
	if (health == 0) {
		animationManager.changeAnim(animations::die);
		return true;
	}
	return false;
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

statuscode GameCharacter::addItem(std::shared_ptr<Item> obj)
{
	if (actionPoints >= AP_COST_PICK_ITEM) {
		if (inventory.add(obj)) {
			actionPoints -= AP_COST_PICK_ITEM;
			return nothing;
		} else return inventory_full;
	}
	return not_enough_ap;
}

bool GameCharacter::removeSelectedItem()
{
	if (actionPoints >= AP_COST_DROP_ITEM) {
		inventory[selectedItemIdx] = std::make_shared<Item>(Item());
		if (selectedItemIdx == selectedWeaponIdx)
		{
			unequipCharacter();
		}
		selectedItemIdx = -1;
		actionPoints -= AP_COST_DROP_ITEM;
		return true;
	}
	return false;
}

statuscode GameCharacter::useSelected()
{
	if (actionPoints >= AP_COST_USE) {
		if (selectedItemIdx == -1) return item_none_selected;
		if (selectedItemIdx == selectedWeaponIdx) {
			unequipCharacter();
			return item_weapon_unequipped;
		}
		statuscode status = nothing;

		switch (inventory[selectedItemIdx]->getType()) {
		case Type_Weapon:
			selectedWeaponIdx = selectedItemIdx;
			equippedWeapon = std::dynamic_pointer_cast<Weapon>(inventory[selectedItemIdx]);
			actionPoints -= AP_COST_USE;
			status = item_weapon_equipped;
			break;
		case Type_Health:
			if (getHitpoints() == getMaxHitpoints()) return item_max_health;
			health = std::min(getMaxHitpoints(), getHitpoints() + std::dynamic_pointer_cast<Health>(inventory[selectedItemIdx])->getHealingAmount());
			actionPoints -= AP_COST_USE;
			status = item_healed;
			break;
		case Type_Powerup:
			switch (std::dynamic_pointer_cast<Powerup>(inventory[selectedItemIdx])->getStatType()) {
			case Powerup_ActionPoints:
				maxActionPoints += std::dynamic_pointer_cast<Powerup>(inventory[selectedItemIdx])->getIncreaseAmount();
				actionPoints += std::dynamic_pointer_cast<Powerup>(inventory[selectedItemIdx])->getIncreaseAmount();
				status = item_increase_ap;
				break;
			case Powerup_Health:
				maxHealth += std::dynamic_pointer_cast<Powerup>(inventory[selectedItemIdx])->getIncreaseAmount();
				health += std::dynamic_pointer_cast<Powerup>(inventory[selectedItemIdx])->getIncreaseAmount();
				status = item_increase_hp;
				break;
			case Powerup_LineOfSight:
				lengthofSight += std::dynamic_pointer_cast<Powerup>(inventory[selectedItemIdx])->getIncreaseAmount();
				status = item_increase_los;
				break;
			default:
				break;
			actionPoints -= AP_COST_USE;
			break;
			}

		default:
			status = item_unusable;
			break;

		}
		if (inventory[selectedItemIdx]->isConsumeable()) inventory[selectedItemIdx]->removeAmount(1);
		if (inventory[selectedItemIdx]->getAmount() == 0) {
			inventory.remove(selectedItemIdx);
		}
		return status;
	}
	return not_enough_ap;
}

void GameCharacter::unequipCharacter()
{
	selectedWeaponIdx = -1;
	equippedWeapon = std::make_shared<Hands>(Hands());
}

unsigned int GameCharacter::getAmmoAmount(AmmoType ammotype, unsigned int neededAmount)
{
	std::shared_ptr<Item> foundAmmo = getInventory().findAmmo(ammotype);
	unsigned int availableAmmo = foundAmmo->getAmount();
	if (availableAmmo == 0) return 0;

	if (neededAmount != 0) {
		if (availableAmmo >= neededAmount) {
			foundAmmo->removeAmount(neededAmount);
			if (foundAmmo->getAmount() == 0) {
				inventory.remove(foundAmmo);
			}
			return neededAmount;
		} else {
			inventory.remove(foundAmmo);
			return availableAmmo;
		}
	}
	return availableAmmo;
}

statuscode GameCharacter::reloadWeapon()
{
	if (getEquipped()->getAmmoType() == Ammo_None) return nothing;

	if (actionPoints >= AP_COST_RELOAD) {
		unsigned int availableAmmo = getAmmoAmount(getEquipped()->getAmmoType(), getEquipped()->getReloadAmount());
		if (availableAmmo > 0) {
			getEquipped()->reload(availableAmmo);
			actionPoints -= AP_COST_RELOAD;
			return shoot_reload;
		} else return shoot_no_ammo;
	}
	return not_enough_ap;
}
