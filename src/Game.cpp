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

bool Game::characterMove(std::vector<GameCharacter>::iterator it, sf::Vector2i direction) {
	sf::Vector2i target_pos = (sf::Vector2i) it->getPosition() + direction;
	if (!getGrid()(target_pos).isSolid() && std::all_of(characters.begin(), characters.end(), 
			[target_pos](GameCharacter gc){ return (sf::Vector2i) gc.getPosition() != target_pos; })) {
		it->moveTo(direction);
		return true;
	} else {
		return false;
	}
}

bool Game::characterMoveLeft(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(-1, 0);
	return characterMove(it, dir);
}

bool Game::characterMoveRight(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(1, 0);
	return characterMove(it, dir);
}

bool Game::characterMoveUp(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(0, -1);
	return characterMove(it, dir);
}

bool Game::characterMoveDown(std::vector<GameCharacter>::iterator it) {
	sf::Vector2i dir(0, 1);
	return characterMove(it, dir);
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


