#ifndef TILE_H
#define TILE_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>

enum TileGround { black = 0, dirt = 1, grass = 2, wood = 3, stone = 4, metal = 5 };

enum TileBlock { air = 0, wall = 1, tree = 2, bush = 3 };

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