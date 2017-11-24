#ifndef TILE_H
#define TILE_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

enum TileGround { dirt, grass, wood, stone, metal, black };

enum TileBlock { air, wall, tree, bush };

class Tile {
public:
	Tile() : Tile(dirt, air) { }

	Tile(TileGround ground) : Tile(ground, air) { }

	Tile(TileGround ground, TileBlock block) {
		setTile(ground, block);
	}

	sf::Texture& getTexture();
	TileGround getGround() const;
	TileBlock getBlock() const;
	const std::vector<int>& getItems() const;
	bool removeItem(int item);
	bool addItem(int obj);
	bool isSolid() const;
	void setTile(TileGround tg, TileBlock tb);

private:
	TileGround ground;
	TileBlock block;
	//temporarily vector of ints since there's no item class yet
	std::vector<int> items;
	sf::Texture texture;
};

std::ostream& operator<<(std::ostream& out, const Tile& t);

#endif