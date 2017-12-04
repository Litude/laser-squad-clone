#include "Game.hpp"

void Game::endTurn() {
	(playerTurnIdx == 1) ? playerTurnIdx = 2 : playerTurnIdx = 1;
	selectedCharacter = characters.end();
	for (auto &character: characters) {
		character.resetActionPoints();
	}
}

bool Game::addCharacter(sf::Vector2u position, unsigned int team) {
	if (team == 1 || team == 2) {
		characters.push_back(GameCharacter(position, team));
		return true;
	}
	else {
		return false;
	}
}

bool Game::characterMove(gc_iterator it, sf::Vector2i direction) {
	sf::Vector2i target_pos = (sf::Vector2i) it->getPosition() + direction;
	if (!getGrid()(target_pos).isSolid() && std::all_of(characters.begin(), characters.end(), 
			[target_pos](GameCharacter gc){ return (sf::Vector2i) gc.getPosition() != target_pos; })) {
		it->moveTo(direction);
		return true;
	} else {
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
	sf::Vector2i dir(0, 1);
	return characterMove(it, dir);
}

bool Game::characterPickUpItem(std::vector<GameCharacter>::iterator it) {
	if (getSelectedCharacter()->addItem(getGrid().getTile(getSelectedCharacter()->getPosition().x, getSelectedCharacter()->getPosition().y).getTopItem())) {
		getGrid().getTile(getSelectedCharacter()->getPosition().x, getSelectedCharacter()->getPosition().y).popItem();
		return true;
	}
	return false;
}

bool Game::characterDropItem(std::vector<GameCharacter>::iterator it) {
	getGrid().getTile(getSelectedCharacter()->getPosition().x, getSelectedCharacter()->getPosition().y).addItem(getSelectedCharacter()->getInventory()[getSelectedCharacter()->getSelectedItemIndex()]);
	if (getSelectedCharacter()->removeSelectedItem()) {
		return true;
	}
	else {
		// Could not drop the item, probably not enough AP so we need to pop the item we just added to the map
		getGrid().getTile(getSelectedCharacter()->getPosition().x, getSelectedCharacter()->getPosition().y).popItem();
		return false;
	}
}

// Trace line from gamecharacter location to target, returning
// first tile that blocks tracing
const sf::Vector2u Game::traceFromCharacter(gc_iterator it, sf::Vector2u target) {
	//clamp to bounds
	sf::Vector2u within_bounds;
	within_bounds.x = (target.x < grid.getWidth() ? target.x : grid.getWidth());
	within_bounds.y = (target.y < grid.getHeight() ? target.y : grid.getHeight());
	std::vector<sf::Vector2u> pierced = Util::traceLine(static_cast<sf::Vector2i>(it->getPosition()) , static_cast<sf::Vector2i>(within_bounds));
	sf::Vector2u endTile;
	if (pierced.size() > 1) {
		endTile = getEndTile(++pierced.begin(), pierced.end());
	} else {
		endTile = it->getPosition();
	}
	return endTile;
}

// Try to shoot at Tile on coordinates sf::Vector2u target
// Actual Tile hit may deviate based on weapon statistics
const std::vector<sf::Vector2u> Game::characterShoot(gc_iterator it, sf::Vector2u target) {
	std::cout << "===================" << std::endl;
	std::cout << "character origin: (" << it->getPosition().x << ", " << it->getPosition().y << ")" << std::endl;
	std::cout << "shoot called with coords: (" << target.x << ", " << target.y << ")" << std::endl;
	// TODO:
	// get weapon behavior from equipped weapon, ie. simple, explosive, several shots
	// get error on target tile from weapon behavior, ie how much actual target tile deviates from selected tile
	std::vector<sf::Vector2u> endTiles;
	int numberOfShots = it->shoot();
	for (int i = 0; i < numberOfShots; ++i) {
		auto deviated = it->getEquipped()->deviate(target);
		auto endTile = traceFromCharacter(it, deviated);
		std::cout << "landed at tile at coords: (" << endTile.x << ", " << endTile.y << ")" << std::endl;
		for (auto &gc : characters) {
			if (gc.getPosition() == endTile) {
				int dmg = it->getEquipped()->getDamage();
				gc.sufferDamage(dmg);
				std::cout << "character suffered " << dmg << " damage" << std::endl;
				break;
			}
		endTiles.push_back(endTile);
		}
		
	}
	std::cout << "Number of shots fired: " << numberOfShots << std::endl;
	return endTiles;
}

// Returns reference to first coordinate located on a coordinate from coords_begin to coords_end
// that contains an object blocking line tracing to target.
// If no elements block tracing, last coordinate is returned.
//
// Blocking elements: game characters and tile blocks
// NOTE: iterator must point to at least one valid element
const sf::Vector2u Game::getEndTile(coord_iterator coords_begin, coord_iterator coords_end, int maxRange) {
	for (auto it = coords_begin; it != coords_end; ++it) {
		if (grid(*it).isSolid() || std::distance(coords_begin, it) >= maxRange || 
			std::any_of(characters.begin(), characters.end(), 
			[it](GameCharacter gc) { return gc.getPosition() == *it; })) return *it;
	}
	return *(--coords_end);
}

std::vector<sf::Vector2u> Game::seenCoordinates(std::vector<GameCharacter>::iterator it){
    int positionX=it->getPosition().x;
    int positionY=it->getPosition().y;
    int length=it->getLengthofSight();
    int x,y;
    std::vector<sf::Vector2u> seen;
    for(int i=0;i<length;i++){
        for(int j=0;j<length;j++){
            x=positionX-length/2+i;
            y=positionY-length/2+j;
            sf::Vector2u coord=sf::Vector2u(x,y);
            if(lineofSight(positionX, positionY, x, y)!=false){
                seen.push_back(coord);
            }
        }
    }
    return seen;
}

bool Game::lineofSight(int x1,int y1,int x2,int y2)  {
    int x,y,dx,dy,dx1,dy1,px,py,xe,ye,i;
    dx=x2-x1;
    dy=y2-y1;
    dx1=abs(dx);
    dy1=abs(dy);
    px=2*dy1-dx1;
    py=2*dx1-dy1;
    if(dy1<=dx1)
    {
        if(dx>=0) {
            x=x1;
            y=y1;
            xe=x2;
        }else{
            x=x2;
            y=y2;
            xe=x1;
        }
        
        sf::Vector2i pos(x,y);
        if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
            return false;
        }
        
        for(i=0;x<xe;i++)   {
            x=x+1;
            if(px<0){
                px=px+2*dy1;
            }else{
                if((dx<0 && dy<0) || (dx>0 && dy>0))
                {
                    y=y+1;
                }else{
                    y=y-1;
                }
                px=px+2*(dy1-dx1);
            }
            sf::Vector2i pos(x,y);
            if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
                return false;
            }
        }
    }else{
        if(dy>=0){
            x=x1;
            y=y1;
            ye=y2;
        }else{
            x=x2;
            y=y2;
            ye=y1;
        }
        sf::Vector2i pos(x,y);
        if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
            return false;
        }
        
        for(i=0;y<ye;i++)   {
            y=y+1;
            if(py<=0)   {
                py=py+2*dx1;
            }else{
                if((dx<0 && dy<0) || (dx>0 && dy>0))    {
                    x=x+1;
                }else{
                    x=x-1;
                }
                py=py+2*(dx1-dy1);
            }
            sf::Vector2i pos(x,y);
            if (getGrid()(pos).isSolid() && !(x2 == x && y2 == y)) {
                return false;
            }
        }
    }
    return true;
}


