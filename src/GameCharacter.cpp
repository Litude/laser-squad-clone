#include "GameCharacter.hpp"
#include <iostream>

void GameCharacter::moveLeft() {
	actualPosition.x -= TILESIZE;
	direction = left;
	moving = true;
}

void GameCharacter::moveRight() {
	actualPosition.x += TILESIZE;
	direction = right;
	moving = true;
}

void GameCharacter::moveUp() {
	actualPosition.y -= TILESIZE;
	direction = up;
	moving = true;
}

void GameCharacter::moveDown() {
	actualPosition.y += TILESIZE;
	direction = down;
	moving = true;
}

void GameCharacter::move() {
	switch (direction) {
	case left:
		--currentPosition.x;
		++animation;
		if (currentPosition.x == actualPosition.x) { moving = false; animation = 0;} ;
		break;
	case right:
		++currentPosition.x;
		++animation;
		if (currentPosition.x == actualPosition.x) { moving = false; animation = 0; };
		break;
	case up:
		--currentPosition.y;
		++animation;
		if (currentPosition.y == actualPosition.y) { moving = false; animation = 0; };
		break;
	case down:
		++currentPosition.y;
		++animation;
		if (currentPosition.y == actualPosition.y) { moving = false; animation = 0; };
		break;
	}
}

//void GameCharacter::move() {
//	switch (direction) {
//	case left:
//		--positionX;
//		++animation;
//		if (positionX % TILESIZE == 0) { moving = false; animation = 0; };
//		break;
//	case right:
//		++positionX;
//		++animation;
//		if (positionX % TILESIZE == 0) { moving = false; animation = 0; };
//		break;
//	case up:
//		--positionY;
//		++animation;
//		if (positionY % TILESIZE == 0) { moving = false; animation = 0; };
//		break;
//	case down:
//		++positionY;
//		++animation;
//		if (positionY % TILESIZE == 0) { moving = false; animation = 0; };
//		break;
//	}
//}
