#ifndef GRIDLOADER_H
#define GRIDLOADER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "libs/json.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Game.hpp"

namespace jreader {

	static std::map<std::string, TileBlock> block = {{"wall", wall}, {"tree", tree}, {"bush", bush}, {"air", air}};
	static std::map<std::string, TileGround> ground = {{"grass", grass}, {"dirt", dirt}, {"wood", wood}, {"stone", stone}, {"metal", metal}, {"black", black}};
	static std::vector<std::string> block_str = {"black", "dirt", "grass", "wood", "stone", "metal"};
	static std::vector<std::string> ground_str = {"air", "wall", "tree", "bush"};

	Tile createTile(nlohmann::json& j) {
		return Tile();
	}

	nlohmann::json createTile(Tile& tile) {
		nlohmann:json jt;
		jtile["ground"] = ground_str[t.getGround()];
		jtile["block"] = block_str[t.getBlock()];
		for (int item : tile.getItems()) {
			jtile["items"].push_back(item);
		}
	}

	Grid loadGrid(nlohmann::json& jgrid) {

		unsigned mapSize;
		unsigned mapWidth;

		if (jgrid["size"] == nullptr || jgrid["width"] == nullptr) {
			//throw some exception
			return Grid();
		} else {
			mapSize = (unsigned) jgrid["size"];
			mapWidth = (unsigned) jgrid["width"];		
		}

		std::cout << "mapsize:" << mapSize << " width:" << mapWidth << std::endl;
		Grid g = Grid(mapSize, mapWidth);
		
		int i = 0;
		for (auto j : jgrid["tiles"]) {
			Tile t;
			if (j["block"] == nullptr) {
				t = Tile(ground[j["ground"]]);
			} else {
				t = Tile(ground[j["ground"]], block[j["block"]]);
			}
			g[i] = t;
			++i;
		}

		std::cout << g << std::endl;

		return g;
	}

	Grid loadJSON(std::string filepath) {
		nlohmann::json jloader;

		std::ifstream is(filepath);
		is >> jloader;
		is.close();

		if (jloader["map"] == nullptr) {
			std::cout << "JSON file had no map element. Skipping map load." << std::endl;
			return Grid();
		}

		Grid g = loadGrid(jloader["map"]);

		return g;
	}

	bool writeJSON(Game& game, std::string filepath) {
		nlohmann::json writer;
		writer["map"] = nlohmann::json::array();
		for (auto tile : g.getGrid()) {
			nlohmann::json jtile = {
				jtile = createTile(tile);
			}
			writer["map"].push_back(jtile);
		}
		return true;
	}	

}


#endif