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

std::vector<std::pair<int, int>> Game::seenCoordinates(std::vector<GameCharacter>::iterator it){
    std::vector<std::pair<int, int>> block; //stores unseen coordinates
    int length=8; //size of seen area. Just a random number at the moment. Maybe could be placed to private of game class
    int x,y; //for help
    
    int positionX=it->getPosition().x;
    int positionY=it->getPosition().y;
    
    
    std::pair<int, int> pari; //for help
    std::vector<std::pair<int, int>> area; //stores seen coordinates
    
    //First area(x,y)(+,-)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            x=positionX+j;
            y=positionY-i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                
                int length2=length-(positionY-y);
                
                int k=2;
                
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        x2=x+j2;
                        y2=y-1-i2;
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                        
                    }
                    
                }
            }
        }
    }
    
    
    
    
    //Second area(x,y)(-,-)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            x=positionX-j;
            y=positionY-i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                
                int length2=length-(positionY-y);
                
                int k=2;
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        x2=x-j2;
                        y2=y-1-i2;
                        
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                    }
                }
            }
            
        }
    }
    
    //Third area(x,y)(+,+)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            x=positionX+j;
            y=positionY+i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                
                int length2=length-(y-positionY);
                
                int k=2;
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        x2=x+j2;
                        y2=y+1+i2;
                        
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                    }
                }
            }
            
        }
    }
    
    //Fourth area (x,y)(-,+)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            x=positionX-j;
            y=positionY+i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                
                int length2=length-(y-positionY);
                
                int k=2;
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        
                        x2=x-j2;
                        y2=y+1+i2;
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                    }
                }
            }
            
        }
    }
    
    //Fifth area (y,x)(-,-)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            y=positionX-j;
            x=positionY-i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                
                int length2=length-(positionX-x)-1;
                
                int k=2;
                
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        
                        y2=y-j2;
                        x2=x-1-i2;
                        
                        
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                    }
                }
            }
            
        }
    }
    
    //Sixth area (y,x)(+,-)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            y=positionX+j;
            x=positionY-i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                
                int length2=length-(positionX-x)-1;
                
                int k=2;
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        
                        y2=y+j2;
                        x2=x-1-i2;
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                    }
                }
            }
            
        }
    }
    
    //Seventh area (y,x)(-,+)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            y=positionX-j;
            x=positionY+i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                std::cout << "seventh" << std::endl;
                int length2=length-(x-positionX);
                
                
                int k=2;
                std::cout << "fifth" << std::endl;
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        
                        y2=y-j2;
                        x2=x+1+i2;
                        
                        
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                    }
                }
            }
            
        }
    }
    
    //Eight area (y,x)(+,+)
    for(int i=0;i<length;i++) {
        for(int j=0;j<i+1;j++) {
            
            //basic seen area
            y=positionX+j;
            x=positionY+i;
            
            pari=std::make_pair(x,y);
            area.push_back(pari);
            sf::Vector2i pos(x,y);
            //Checks objects and create vector including unvisible coordinates
            if(getGrid()(pos).isSolid())  {
                int x2;
                int y2;
                
                int i2,j2;
                
                int length2=length-(x-positionX);
                
                int k=2;
                
                
                for(i2=0;i2<length2;i2++) {
                    
                    if(i2%2==0){
                        k=k-1;
                    }
                    
                    for(j2=0;j2<i2+k;j2++) {
                        
                        y2=y+j2;
                        x2=x+1+i2;
                        
                        
                        std::pair<int, int> pari2;
                        pari2=std::make_pair(x2,y2);
                        block.push_back(pari2);
                    }
                }
            }
            
        }
    }
    
    
    //delete duplicate coordinates
    std::sort(block.begin(), block.end());
    std::vector<std::pair<int, int>>::iterator iter;
    iter = std::unique(block.begin(), block.end());
    block.resize( std::distance(block.begin(),iter) );
    
    std::sort(area.begin(), area.end());
    std::vector<std::pair<int, int>>::iterator iter2;
    iter2 = std::unique(area.begin(), area.end());
    area.resize( std::distance(area.begin(),iter2) );
    
    
    //delete unseen coordinates
    for(auto it=area.begin();it!=area.end();it++)   {
        for(auto it2=block.begin();it2!=block.end();it2++) {
            if(*it==*it2)   {
                area.erase(it);
            }
        }
    }
    return area;
}
