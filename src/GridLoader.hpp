#ifndef GRIDLOADER_H
#define GRIDLOADER_H

#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include "libs/json.hpp"
#include "Grid.hpp"
#include "alt_Tile.hpp"
//#include "Game.hpp"

namespace jreader {

	static std::map<std::string, TileBlock> block = {{"wall", wall}, {"tree", tree}, {"bush", bush}, {"air", air}};
	static std::map<std::string, TileGround> ground = {{"grass", grass}, {"dirt", dirt}, {"wood", wood}, {"stone", stone}, {"metal", metal}, {"black", black}};

	Tile createTile(nlohmann::json j) {
		return Tile();
	}

	Grid loadGrid(nlohmann::json map);

	Grid loadJSON(std::string filepath) {
		nlohmann::json jloader;

		std::ifstream is(filepath);
		is >> jloader;
		is.close();

		if (jloader["map"] == nullptr) {
			return Grid();
		}

		Grid g = loadGrid(jloader["map"]);

		return g;
	}

	Grid loadGrid(nlohmann::json jgrid) {

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

		unsigned k = 0;
		for (auto it = g.begin(); it != g.end(); ++it) {
			std::cout << *it;
			++k;
			if (k == g.getWidth()) {
				std::cout << std::endl;
				k = 0;
			}
		}

		return g;
	}

	bool writeJSON(std::string filepath) {
		return true;
	}	

}



#endif