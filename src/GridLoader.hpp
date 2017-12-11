#ifndef GRIDLOADER_H
#define GRIDLOADER_H

#include <fstream>
#include <iostream>
#include <typeinfo>
#include <map>
#include <string>
#include "libs/json.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Item.hpp"
#include "Game.hpp"
#include "Exceptions.hpp"

namespace jreader {

	static std::map<std::string, TileBlock> block = {{"wall", wall}, {"tree", tree}, {"bush", bush}, {"air", air}};
	static std::map<std::string, TileGround> ground = {{"grass", grass}, {"dirt", dirt}, {"wood", wood}, {"stone", stone}, {"metal", metal}, {"black", black}};
	//static const std::map<std::string, ItemIcon> itemCreator = {{"Small Health Pack"}, {"Large Health Pack"}}
	static std::vector<std::string> ground_str = {"black", "dirt", "grass", "wood", "stone", "metal"};
	static std::vector<std::string> block_str = {"air", "wall", "tree", "bush"};

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

	inline Tile createTile(nlohmann::json& j) {
		if (j["block"] == nullptr) return Tile();
		auto gr = get(ground, j["ground"], grass);
		auto bl = get(block, j["block"], air);
		return Tile(gr, bl);
	}

	inline nlohmann::json createTile(Tile& tile) {
		nlohmann::json jtile;
		jtile["ground"] = ground_str[tile.getGround()];
		jtile["block"] = block_str[tile.getBlock()];
		for (auto item : tile.getItems()) {
			jtile["items"].push_back((*item).getName());
		}
		return jtile;
	}

	inline Grid loadGrid(nlohmann::json& jgrid) {

		unsigned mapSize;
		unsigned mapWidth;

		try {
			if (jgrid["size"].is_number_unsigned() && jgrid["width"].is_number_unsigned()) {
				mapSize = jgrid["size"];
				mapWidth = jgrid["width"];
			} else {
				throw JSONLoadException();
			}
		} catch (const nlohmann::detail::exception& e) {
			throw JSONLoadException();
		}

		std::cout << "mapsize:" << mapSize << " width:" << mapWidth << std::endl;
		Grid g = Grid(mapSize, mapWidth);
		
		unsigned u = 0;
		
		for (auto j : jgrid["tiles"]) {
			if (u > mapSize) break;
			Tile t = createTile(j);
			g[u] = t;
			++u;
		}
		
		std::cout << g << std::endl;

		return g;
	}

	inline Grid loadJSON(std::string filepath) {
		nlohmann::json jloader;
		std::ifstream is;
		is.exceptions( std::ifstream::failbit | std::ifstream::badbit );

		try {
			is.open("levels/" + filepath + ".json");
			is >> jloader;
			is.close();
		} catch (const nlohmann::detail::parse_error& pe) {
			throw JSONLoadException();
		} catch (const std::ifstream::failure& e) {
			throw JSONLoadException();
		}

		if (jloader["map"] == nullptr) {
			std::cout << "JSON file had no map element. Skipping map load." << std::endl;
			throw JSONLoadException();
		}

		Grid g = loadGrid(jloader["map"]);

		Game game;

		game.setGrid(g);

		return g;
	}

	inline bool writeJSON(Game& game, std::string filename) {
		nlohmann::json writer;
		writer["map"];
		writer["map"]["size"] = game.getGrid().getSize();
		writer["map"]["width"] = game.getGrid().getWidth();
		writer["map"]["tiles"] = nlohmann::json::array();
		writer["characters"] = nlohmann::json::array();

		for (auto tile : game.getGrid()) {
			nlohmann::json jtile = createTile(tile);
			writer["map"]["tiles"].push_back(jtile);
		}
		for (auto gc : game.getCharacters()) {
			nlohmann::json jchar;
			jchar["team"] = gc.getTeam();
			jchar["health"] = gc.getHitpoints();
			jchar["inventory"] = nlohmann::json::array();
			for (auto item : gc.getInventory()) {
				jchar["inventory"].push_back(item->getName());
			}
			writer["characters"].push_back(jchar);
		}

		std::ofstream os;
		os.exceptions( std::ifstream::failbit | std::ifstream::badbit );
		try {
			os.open("levels/" + filename + ".json");
			os << writer;
			os.close();
		} catch (std::ofstream::failure& e) {
			throw JSONLoadException();
		}
		return true;
	}	

}


#endif