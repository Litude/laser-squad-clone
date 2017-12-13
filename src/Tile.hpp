#ifndef TILE_H
#define TILE_H

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include "Item.hpp"

enum TileGround { black = 0, dirt = 1, grass = 2, wood = 3, stone = 4, metal = 5 };

enum TileBlock { air = 0, wall = 1, tree = 2, bush = 3, trophy = 4, small_tree = 5, cherry_tree = 6, rock = 7 };

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
	//bool removeItem(int item);
	bool isSolid() const;
	void setTile(TileGround tg, TileBlock tb);

	/*template<typename T>
	bool addItem(T& item) {
		items.push_back(std::make_shared<T>(item));
		return true;
	}*/

	bool addItem(std::shared_ptr<Item> obj) {
		items.push_back(obj);
		return true;
	}

private:
	TileGround ground;
	TileBlock block;
	std::vector<std::shared_ptr<Item>> items;
};

std::ostream& operator<<(std::ostream& out, const Tile& t);

#endif