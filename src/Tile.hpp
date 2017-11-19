#ifndef LASER_TILE_HPP
#define LASER_TILE_HPP

#include <string>
#include <SFML/Graphics.hpp>

class Tile {
public:
	Tile();
	sf::Texture&	getTexture() { return texture; }
	bool			isSolid() { return solid; }
	void			setTile(std::string textureFileName, bool newSolidFlag);

private:
	bool			solid;
	sf::Texture		texture;
};

#endif