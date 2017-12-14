#ifndef TILE_H
#define TILE_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Item.hpp"

enum TileGround {
	black = 0,
	dirt = 1,
	grass = 2,
	wood = 3,
	stone = 4,
	metal = 5,
	wooden_floor = 6,
	metal_floor = 7,
	yellow_woodfloor = 8,
	alienblood_floor = 9,
	marble_floor = 10,
	gold_floor = 11,
	gold_shape = 12,
	checkerboard = 13,
	road_rocky = 14,
	road_bricks = 15
};

enum TileBlock {
	air = 0,
	wall = 1,
	tree = 2,
	bush = 3,
	trophy = 4,
	small_tree = 5,
	cherry_tree = 6,
	rock = 7,
	barrels = 8,
	stove = 9,
	stone_head = 10,
	toilet = 11
};

class Tile {
public:
	Tile() : Tile(dirt, air) { }

	Tile(TileGround ground) : Tile(ground, air) { }

	Tile(TileGround ground, TileBlock block) {
		setTile(ground, block);
	}

	TileGround getGround() const;
	TileBlock getBlock() const;
	ItemIcon getTopItemIcon() const;
	std::shared_ptr<Item> getTopItem();
	void popItem();
	const std::vector<std::shared_ptr<Item>>& getItems() const;
	bool isSolid() const;
	void setTile(TileGround tg, TileBlock tb);
	bool addItem(std::shared_ptr<Item> obj);

private:
	TileGround ground;
	TileBlock block;
	std::vector<std::shared_ptr<Item>> items;
};

std::ostream& operator<<(std::ostream& out, const Tile& t);

#endif