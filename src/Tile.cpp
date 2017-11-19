#include "Tile.hpp"
#include <SFML/Graphics.hpp>
#include "constants.hpp"
#include <iostream>

Tile::Tile() {
	if (!texture.loadFromFile("img/tile_grass.png")) {
		std::cout << "Could not load 'img/tile_grass.png'\n";
	}
	solid = false;
}

//Temp function for map collision detection, must be called before interface drawing is initialized to be visible on the map
void Tile::setTile(std::string textureFileName, bool newFlag) {
	if (!texture.loadFromFile(textureFileName)) {
		std::cout << "Could not load '" << textureFileName << "'\n";
	}
	solid = newFlag;
}