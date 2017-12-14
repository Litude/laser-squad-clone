#include "Game.hpp"

void Game::endTurn() {
	if (playerTurnIdx == 2) ++turnNo;
	(playerTurnIdx == 1) ? playerTurnIdx = 2 : playerTurnIdx = 1;
	recalculateLOS = true;
	selectedCharacter = characters.end();
	for (auto &character : characters) {
		character.resetActionPoints();
	}
	statusMessage.clearStatusMessage();
	updateGameState();
}

bool Game::addCharacter(GameCharacter gc) {
	characters.push_back(gc);
	return true;
}

bool Game::addCharacter(sf::Vector2u position, unsigned int team) {
	if (team == 1 || team == 2) {
		characters.push_back(GameCharacter(position, team));
		return true;
	} else {
		return false;
	}
}

bool Game::removeCharacter(sf::Vector2u position) {
	for (auto it = getCharacters().begin(); it != getCharacters().end(); it++) {
		if (it->getPosition() == position) {
			getCharacters().erase(it);
			return true;
		}
	}
	return false;
}

bool Game::characterMove(gc_iterator it, sf::Vector2i direction) {
	sf::Vector2i target_pos = (sf::Vector2i) it->getPosition() + direction;
	if (!getGrid()(target_pos).isSolid() && std::all_of(characters.begin(), characters.end(),
		[target_pos](GameCharacter gc) { return (sf::Vector2i) gc.getPosition() != target_pos; })) {
		if (!it->moveTo(direction)) statusMessage.setStatusMessage(MSG_NOT_ENOUGH_AP, SEVERITY_CRITICAL);
		recalculateLOS = true;
		updateGameState();
		return true;
	}
	else {
		return false;
	}
}

bool Game::characterMoveLeft(gc_iterator it) {
	sf::Vector2i dir(-1, 0);
	return characterMove(it, dir);
}

bool Game::characterMoveRight(gc_iterator it) {
	sf::Vector2i dir(1, 0);
	return characterMove(it, dir);
}

bool Game::characterMoveUp(gc_iterator it) {
	sf::Vector2i dir(0, -1);
	return characterMove(it, dir);
}

bool Game::characterMoveDown(gc_iterator it) {
	//std::cout << "Moved\n";
	sf::Vector2i dir(0, 1);
	return characterMove(it, dir);
}

bool Game::characterPickUpItem() {
	if (getGrid()(getSelectedCharacter()->getPosition()).getTopItem()->getType() == Type_None) {
		statusMessage.setStatusMessage(MSG_NOTHING_TO_PICK, SEVERITY_INFORMATION);
		return false;
	}
	switch (getSelectedCharacter()->addItem(getGrid()(getSelectedCharacter()->getPosition()).getTopItem())) {
	case not_enough_ap:
		statusMessage.setStatusMessage(MSG_NOT_ENOUGH_AP, SEVERITY_CRITICAL);
		return false;
	case inventory_full:
		statusMessage.setStatusMessage(MSG_INVENTORY_FULL, SEVERITY_CRITICAL);
		return false;
	default:
		getGrid()(getSelectedCharacter()->getPosition()).popItem();
		updateGameState();
		return true;
	}
	return false;
}

bool Game::characterDropItem() {
	if (getSelectedCharacter()->getSelectedItemIndex() == -1) {
		statusMessage.setStatusMessage(MSG_ITEM_NONE_SELECTED, SEVERITY_INFORMATION);
		return false;
	}

	getGrid()(getSelectedCharacter()->getPosition()).addItem(getSelectedCharacter()->getInventory()[getSelectedCharacter()->getSelectedItemIndex()]);
	if (getSelectedCharacter()->removeSelectedItem()) {
		updateGameState();
		return true;
	}
	else {
		// Could not drop the item, probably not enough AP so we need to pop the item we just added to the map
		statusMessage.setStatusMessage(MSG_NOT_ENOUGH_AP, SEVERITY_CRITICAL);
		getGrid()(getSelectedCharacter()->getPosition()).popItem();
		return false;
	}
}

void Game::characterUseItem(gc_iterator it) {
	//Pass-through function to enable status listening...
	switch (it->useSelected()) {
	case not_enough_ap:
		statusMessage.setStatusMessage(MSG_NOT_ENOUGH_AP, SEVERITY_CRITICAL);
		break;
	case item_weapon_equipped:
		statusMessage.setStatusMessage(MSG_WEAPON_EQUIPPED, SEVERITY_INFORMATION);
		break;
	case item_weapon_unequipped:
		statusMessage.setStatusMessage(MSG_WEAPON_UNEQUIPPED, SEVERITY_INFORMATION);
		break;
	case item_unusable:
		statusMessage.setStatusMessage(MSG_ITEM_UNUSABLE, SEVERITY_INFORMATION);
		break;
	case item_healed:
		statusMessage.setStatusMessage(MSG_ITEM_HEALED, SEVERITY_INFORMATION);
		break;
	case item_max_health:
		statusMessage.setStatusMessage(MSG_ITEM_MAX_HEALTH, SEVERITY_INFORMATION);
		break;
	case item_none_selected:
		statusMessage.setStatusMessage(MSG_ITEM_NONE_SELECTED, SEVERITY_INFORMATION);
		break;
	case item_increase_ap:
		statusMessage.setStatusMessage(MSG_ITEM_INCREASE_AP, SEVERITY_BOOST);
		break;
	case item_increase_hp:
		statusMessage.setStatusMessage(MSG_ITEM_INCREASE_HP, SEVERITY_BOOST);
		break;
	case item_increase_los:
		statusMessage.setStatusMessage(MSG_ITEM_INCREASE_LOS, SEVERITY_BOOST);
		recalculateLOS = true;
		break;
	default:
		break;
	}
}

void Game::characterDropAllItems(GameCharacter& gc) {
	//Called when a character is killed to drop its inventory contents
	for (auto it = gc.getInventory().begin(); it != gc.getInventory().end(); ++it) {
		if ((*it)->getType() == Type_None) continue;
		getGrid()(gc.getPosition()).addItem(*it);
		*it = std::make_shared<Item>(Item()); //remove item from inventory after dropping

	}
}

// Trace line with weapon max range
const sf::Vector2u Game::traceLineWithWeapon(gc_iterator gc, sf::Vector2u target) {
	auto weapon = gc->getEquipped();
	weapon->clampToMaxRange(gc->getPosition(), target);
	return traceFromCharacter(gc, target, true);
}

// Trace line from gamecharacter location to target, returning
// first tile that blocks tracing
const sf::Vector2u Game::traceFromCharacter(gc_iterator gc, sf::Vector2u target, bool ignoreCharacters) {
	//clamp to bounds
	sf::Vector2u within_bounds;
	within_bounds.x = (target.x < grid.getWidth() ? target.x : grid.getWidth() - 1);
	within_bounds.y = (target.y < grid.getHeight() ? target.y : grid.getHeight() - 1);
	std::vector<sf::Vector2u> pierced = Util::traceLine(static_cast<sf::Vector2i>(gc->getPosition()), static_cast<sf::Vector2i>(within_bounds));
	sf::Vector2u endTile;
	if (pierced.size() > 1) {
		endTile = getEndTile(++pierced.begin(), pierced.end(), ignoreCharacters);//gc->getEquipped()->getRange() - 1);
	} else {
		endTile = gc->getPosition();
	}
	return endTile;
}

// Try to shoot at Tile on coordinates sf::Vector2u target
// Actual Tile hit may deviate based on weapon statistics
const std::vector<sf::Vector2u> Game::characterShoot(gc_iterator it, sf::Vector2u target) {
	std::cout << "===================" << std::endl;
	std::cout << "character origin: (" << it->getPosition().x << ", " << it->getPosition().y << ")" << std::endl;
	std::cout << "shoot called with coords: (" << target.x << ", " << target.y << ")" << std::endl;

	std::vector<sf::Vector2u> endTiles;
	int numberOfShots = 0;
	switch (it->shoot(numberOfShots)) {
	case not_enough_ap:
		statusMessage.setStatusMessage(MSG_NOT_ENOUGH_AP, SEVERITY_CRITICAL);
		break;
	case shoot_reload:
		statusMessage.setStatusMessage(MSG_RELOADING, SEVERITY_INFORMATION);
		break;
	case shoot_no_ammo:
		statusMessage.setStatusMessage(MSG_NOT_ENOUGH_AMMO, SEVERITY_CRITICAL);
		break;
	default:
		break;
	}

	auto weapon = it->getEquipped();

	for (int i = 0; i < numberOfShots; ++i) {
		weapon->clampToMaxRange(it->getPosition(), target);
		auto deviated = weapon->deviate(target);
		auto endTile = traceFromCharacter(it, deviated);
        auto affectedTiles = getAffected(endTile, weapon->getArea());
		std::cout << "landed at tile at coords: (" << endTile.x << ", " << endTile.y << ")" << std::endl;
		for (auto &gc : characters) {
			if (std::find(affectedTiles.begin(), affectedTiles.end(), gc.getPosition()) != affectedTiles.end()) {
				int dmg = weapon->getDamage();
				if (gc.sufferDamage(dmg)) {
					characterDropAllItems(gc);
					recalculateLOS = true;
				}
				std::cout << "character suffered " << dmg << " damage" << std::endl;
				//break;
			}
		}
		endTiles.push_back(endTile);
	}

	std::cout << "Number of shots fired: " << numberOfShots << std::endl;
	it->checkStackableWeaponAmmo();
	updateGameState();
	return endTiles;
}

// Returns reference to first coordinate located on a coordinate from coords_begin to coords_end
// that contains an object blocking line tracing to target.
// If no elements block tracing, last coordinate is returned.
//
// Blocking elements: game characters and tile blocks
// NOTE: iterator must point to at least one valid element
const sf::Vector2u Game::getEndTile(coord_iterator coords_begin, coord_iterator coords_end, bool ignoreCharacters) {
	for (auto it = coords_begin; it != coords_end; ++it) {
		if (grid(*it).isSolid()) return *it;
		if (!ignoreCharacters && std::any_of(characters.begin(), characters.end(),
			[it](GameCharacter gc) { return gc.getPosition() == *it; })) return *it;
	}
	return *(--coords_end);
}

// Get affected tiles around coordinate coord
// If area is 0, only the original tile is returned.
const std::vector<sf::Vector2u> Game::getAffected(sf::Vector2u& coord, int area) {
    std::vector<sf::Vector2u> affected;
    if (area == 0) {
        affected.push_back(coord);
    } else {
        for (int y = -area; y <= area; ++y) {
            for (int x = -area; x <= area; ++x) {
                int sx = ((int) coord.x) + x;
                int sy = ((int) coord.y) + y;
                sf::Vector2i single(sx, sy);
                if (grid.within_bounds(single))
                    affected.push_back(static_cast<sf::Vector2u>(single));
            }
        }
    }
    return affected;
}

std::vector<sf::Vector2u> Game::seenCoordinates(gc_iterator it) {
	int positionX = it->getPosition().x;
	int positionY = it->getPosition().y;
	int length = it->getLengthofSight();
	int x, y;
	std::vector<sf::Vector2u> seen;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < length; j++) {
			x = positionX - length / 2 + i;
			y = positionY - length / 2 + j;
			sf::Vector2u coord = sf::Vector2u(x, y);
			if (lineofSight(positionX, positionY, x, y) != false) {
				seen.push_back(coord);
			}
		}
	}
	return seen;
}

bool Game::lineofSight(int x1, int y1, int x2, int y2) {
	sf::Vector2i pos;
	int x, y, dx, dy, dx1, dy1, px, py, xe, ye, i;
	dx = x2 - x1;
	dy = y2 - y1;
	dx1 = abs(dx);
	dy1 = abs(dy);
	px = 2 * dy1 - dx1;
	py = 2 * dx1 - dy1;
	if (dy1 <= dx1)
	{
		if (dx >= 0) {
			x = x1;
			y = y1;
			xe = x2;
		} else {
			x = x2;
			y = y2;
			xe = x1;
		}

		pos = sf::Vector2i(x, y);
		if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
			return false;
		}

		for (i = 0; x < xe; i++) {
			x = x + 1;
			if (px < 0) {
				px = px + 2 * dy1;
			} else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0))
				{
					y = y + 1;
				} else {
					y = y - 1;
				}
				px = px + 2 * (dy1 - dx1);
			}
			pos = sf::Vector2i(x, y);
			if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
				return false;
			}
		}
	} else {
		if (dy >= 0) {
			x = x1;
			y = y1;
			ye = y2;
		} else {
			x = x2;
			y = y2;
			ye = y1;
		}
		pos = sf::Vector2i(x, y);
		if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
			return false;
		}

		for (i = 0; y < ye; i++) {
			y = y + 1;
			if (py <= 0) {
				py = py + 2 * dx1;
			} else {
				if ((dx < 0 && dy < 0) || (dx > 0 && dy > 0)) {
					x = x + 1;
				} else {
					x = x - 1;
				}
				py = py + 2 * (dx1 - dy1);
			}
			pos = sf::Vector2i(x, y);
			if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
				return false;
			}
		}
	}
	return true;
}

void Game::updateGameState()
{
	if (matchEnded()) {
		gameState = GameState::match_ended;
		setSelectedCharacter(getCharacters().end());
	}
}

void Game::removeDeadCharacters()
{
	//Remove only one character per loop so that indexes stay sane
	//This function is called once per each frame, so characters should still be removed
	//very rapidly

	//Find out selected character index
	int selectedCharIndex = -1;

	for (unsigned int i = 0; i < getCharacters().size(); ++i) {
		if (characters.begin() + i == getSelectedCharacter()) {
			selectedCharIndex = static_cast<int>(i);
			break;
		}
	}

	int i = 0;
	for (auto it = characters.begin(); it != characters.end(); ++i) {
		if (it->shouldBeRemoved()) {
			it = characters.erase(it);
			if (selectedCharIndex == -1) {
				setSelectedCharacter(characters.end());
			} else if (i == selectedCharIndex) {
				setSelectedCharacter(characters.end());
			} else if (i < selectedCharIndex) {
				setSelectedCharacter(characters.begin() + (selectedCharIndex - 1));
			} else {
				setSelectedCharacter(characters.begin() + (selectedCharIndex));
			}
			break;
		} else {
			++it;
		}
	}
}

bool Game::matchEnded()
{
	return isWinner(1) || isWinner(2) || turnNo > maxTurns;
}

bool Game::isWinner(unsigned int playerIdx)
{
	//check for characters alive
	unsigned int numCharacters = 0;
	for (auto &character : getCharacters()) {
		if (character.getTeam() != playerIdx && !(character.isDead())) {
			numCharacters++;
		}
	}
	return numCharacters == 0;
}
