#include "GridLoader.hpp"

std::shared_ptr<Item> jreader::create_item(std::string item_str) {
	if (item_str == "Small Health Pack") {
		return std::make_shared<HealthPackSmall>();
	} else if (item_str == "Large Health Pack") {
		return std::make_shared<HealthPackLarge>();
	} else if (item_str == "Pistol") {
		return std::make_shared<Pistol>();
	} else if (item_str == "Shotgun") {
		return std::make_shared<Shotgun>();
	} else if (item_str == "Uzi") {
		return std::make_shared<Uzi>();
	} else if (item_str == "Rifle") {
		return std::make_shared<Rifle>();
	} else if (item_str == "Double Barrel") {
		return std::make_shared<DoubleBarrel>();
	} else if (item_str == "Throwing Knife") {
		return std::make_shared<Knife>();
	} else if (item_str == "Sword") {
		return std::make_shared<Sword>();
	} else if (item_str == "Rocket Launcher") {
		return std::make_shared<RocketLauncher>();
	} else if (item_str == "9mm Bullets") {
		return std::make_shared<Ammo9mmBullets>();
	} else if (item_str == "12mm Bullets") {
		return std::make_shared<Ammo12mmBullets>();
	} else if (item_str == "50cal Bullets") {
		return std::make_shared<Ammo50calBullets>();
	} else if (item_str == "Rockets") {
		return std::make_shared<AmmoRockets>();
	} else if (item_str == "Crossbow Bolts") {
		return std::make_shared<AmmoCrossbow>();
	} else if (item_str == "Shotgun Shells") {
		return std::make_shared<AmmoShotgunShells>();
	} else if (item_str == "Armor") {
		return std::make_shared<Armor>();
	} else if (item_str == "Bionic Sight") {
		return std::make_shared<Sight>();
	} else if (item_str == "Boots of Running Urgently") {
		return std::make_shared<Boots>();
	} else if (item_str == "Grenade") {
		return std::make_shared<Grenade>();
	} else if (item_str == "Heavy Laser") {
		return std::make_shared<Laser>();
	} else if (item_str == "Laser Pack") {
		return std::make_shared<AmmoLaserPack>();
	} else if (item_str == "Sniper") {
		return std::make_shared<Sniper>();
	} else if (item_str == "Crossbow") {
		return std::make_shared<Crossbow>();
	} else if (item_str == "Minigun") {
		return std::make_shared<Minigun>();
	} else {
		return nullptr;
	}
}

Tile jreader::createTile(nlohmann::json& j) {
	if (j["block"] == nullptr) return Tile();
	auto gr = get(ground, j["ground"], grass);
	auto bl = get(block, j["block"], air);
	Tile tile(gr, bl);

	for (auto& jitem : j["items"]) {
		if (jitem.is_string()) {
			auto item_ptr = create_item(jitem);
			if (item_ptr)
				tile.addItem(item_ptr);
		}
	}
	return tile;
}

nlohmann::json jreader::createTile(Tile& tile) {
	nlohmann::json jtile;
	jtile["ground"] = ground_str[tile.getGround()];
	jtile["block"] = block_str[tile.getBlock()];
	for (auto& item : tile.getItems()) {
		jtile["items"].push_back((*item).getName());
	}
	return jtile;
}

Grid jreader::loadGrid(nlohmann::json& jgrid) {

	unsigned mapSize;
	unsigned mapWidth;

	try {
		if (jgrid["size"].is_number_unsigned() && jgrid["width"].is_number_unsigned()) {
			mapSize = jgrid["size"];
			mapWidth = jgrid["width"];
		} else {
			throw JSONLoadException();
		}
	} catch (nlohmann::detail::exception) {
		throw JSONLoadException();
	}

	std::cout << "mapsize:" << mapSize << " width:" << mapWidth << std::endl;
	Grid g = Grid(mapSize, mapWidth);
	
	unsigned u = 0;
	
	for (auto& j : jgrid["tiles"]) {
		if (u > mapSize) break;
		Tile t = createTile(j);
		g[u] = t;
		++u;
	}
	
	std::cout << g << std::endl;

	return g;
}

std::unique_ptr<GameCharacter> jreader::createCharacter(nlohmann::json& jchar) {
	if (jchar["position"].is_array() && jchar["position"].size() == 2) {
		if (jchar["position"][0].is_number_unsigned() 
			&& jchar["position"][1].is_number_unsigned() 
			&& jchar["team"].is_number_unsigned()) {
			
			unsigned x = jchar["position"][0];
			unsigned y = jchar["position"][1];
			unsigned team = jchar["team"];
			std::unique_ptr<GameCharacter> gc_ptr(new GameCharacter(sf::Vector2u(x, y), team));// GameCharacter gc(sf::Vector2u(x, y), team);
			
			if (jchar["inventory"].is_array()) {
				for (auto& jitem : jchar["inventory"]) {
					auto item_ptr = create_item(jitem);
					if (item_ptr)
						gc_ptr->addItem(item_ptr);
				}
			}
			if (jchar["health"].is_number_unsigned())
				gc_ptr->setHitpoints(jchar["health"]);
			
			return gc_ptr;
		}
	}
	throw JSONLoadException();
}

void jreader::loadCharacters(Game& game, nlohmann::json& characters) {
	if (characters.is_array()) {
		for (auto& jchar : characters) {
			try {
				auto gc_ptr = createCharacter(jchar);
				game.addCharacter(*gc_ptr);
			} catch (JSONLoadException) {
				continue;
			}
		}
	} else {
		throw JSONLoadException();
	}
}

std::shared_ptr<Game> jreader::loadJSON(std::string filepath) {
	nlohmann::json jloader;
	std::ifstream is;
	is.exceptions( std::ifstream::failbit | std::ifstream::badbit );

	try {
		is.open("levels/" + filepath + ".json");
		is >> jloader;
		is.close();
	} catch (nlohmann::detail::parse_error) {
		throw JSONLoadException();
	} catch (std::ifstream::failure) {
		throw JSONLoadException();
	}

	if (jloader["map"] == nullptr) {
		std::cout << "JSON file had no map element. Skipping map load." << std::endl;
		throw JSONLoadException();
	}

	Game game;
	Grid g = loadGrid(jloader["map"]);
	game.setGrid(g);

	loadCharacters(game, jloader["characters"]);

	// End turn to make sure characters have full action points
	// and next turn dialog appears
	//game.endTurn();

	return std::make_shared<Game>(game);
}

bool jreader::writeJSON(Game& game, std::string filename) {
	nlohmann::json writer;
	writer["map"];
	writer["map"]["size"] = game.getGrid().getSize();
	writer["map"]["width"] = game.getGrid().getWidth();
	writer["map"]["tiles"] = nlohmann::json::array();
	writer["characters"] = nlohmann::json::array();

	for (auto& tile : game.getGrid()) {
		nlohmann::json jtile = createTile(tile);
		writer["map"]["tiles"].push_back(jtile);
	}
	for (auto& gc : game.getCharacters()) {
		nlohmann::json jchar;
		jchar["team"] = gc.getTeam();
		jchar["health"] = gc.getHitpoints();
		jchar["inventory"] = nlohmann::json::array();
		jchar["position"] = nlohmann::json::array();
		jchar["position"].push_back(gc.getPosition().x);
		jchar["position"].push_back(gc.getPosition().y);
		for (auto& item : gc.getInventory()) {
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
	} catch (std::ofstream::failure) {
		throw JSONWriteException();
	}
	return true;
}	
