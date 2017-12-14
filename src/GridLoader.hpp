#ifndef GRIDLOADER_H
#define GRIDLOADER_H

#include <fstream>
#include <iostream>
#include <typeinfo>
#include <map>
#include <string>
#include <memory>
#include "libs/json.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Item.hpp"
#include "Game.hpp"
#include "Exceptions.hpp"
#include "Health.hpp"
#include "Weapon.hpp"
#include "Ammo.hpp"
#include "Powerup.hpp"

namespace jreader {

	static std::map<std::string, TileBlock> block = { {"wall", wall}, {"tree", tree}, {"bush", bush}, {"air", air}, {"trophy", trophy}, {"small_tree", small_tree}, {"cherry_tree", cherry_tree}, {"rock", rock}, {"barrels", barrels}, {"stove", stove}, {"stone_head", stone_head}, {"toilet", toilet } };
	static std::map<std::string, TileGround> ground = {{"grass", grass}, {"dirt", dirt}, {"wood", wood}, {"stone", stone}, {"metal", metal}, {"black", black}, {"wooden_floor", wooden_floor }, { "metal_floor", metal_floor },{ "yellow_woodfloor", yellow_woodfloor },{ "alienblood_floor", alienblood_floor },{ "marble_floor", marble_floor },{ "gold_floor", gold_floor },{ "gold_shape", gold_shape },{ "checkerboard", checkerboard },{ "road_rocky", road_rocky },{ "road_bricks", road_bricks } };
	
	// Order must be same as in ground and block enum
	static std::vector<std::string> ground_str = {"black", "dirt", "grass", "wood", "stone", "metal", "wooden_floor", "metal_floor", "yellow_woodfloor", "alienblood_floor", "marble_floor", "gold_floor", "gold_shape", "checkerboard", "road_rocky", "road_bricks"};
	static std::vector<std::string> block_str = {"air", "wall", "tree", "bush", "trophy", "small_tree", "cherry_tree", "rock", "barrels", "stove", "stone_head", "toilet" };

	// Get with default value
	template <typename T>
	T get(std::map<std::string, T>& m, nlohmann::json& key, T defval) {
		if (!key.is_null() && key.is_string()) {
			auto it = m.find(key);
			if (it != m.end())
				return it->second;
		}
		std::cout << "Unknown value, using default." << std::endl;
		return defval;
	}

	std::shared_ptr<Item> create_item(std::string item_str);
	Tile createTile(nlohmann::json& j);
	nlohmann::json createTile(Tile& tile);
	Grid loadGrid(nlohmann::json& jgrid);
	std::unique_ptr<GameCharacter> createCharacter(nlohmann::json& jchar);
	void loadCharacters(Game& game, nlohmann::json& characters);
	std::shared_ptr<Game> loadJSON(std::string filepath);
	bool writeJSON(Game& game, std::string filename);
}


#endif