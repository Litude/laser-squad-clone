#include <iostream>
#include "gtest/gtest.h"

#include "../Game.hpp"
#include "../GridLoader.hpp"

TEST(GridLoader, CanSaveMap) {
	int xSize = 15;
	int ySize = 15;

	Game game = Game();

	game.initializeGrid(xSize, ySize);
	// Add one solid block for collision testing
	game.getGrid().getTile(7, 7).setTile(TileGround::stone, TileBlock::wall);

	// Add characters
	// Team 1
	game.addCharacter(sf::Vector2u(7, 6), 1);
	// Team 2
	game.addCharacter(sf::Vector2u(7, 8), 2);

	// Add item on the map
	game.getGrid().getTile(7, 8).addItem(std::make_shared<Shotgun>(Shotgun()));

	// Try saving
	bool saveResult = jreader::writeJSON(game, "unittest_level");

	EXPECT_EQ(saveResult, true);
}

TEST(GridLoader, CanLoadSavedMap) {
	// Test loading the map that was just saved
	auto game = jreader::loadJSON("unittest_level");

	EXPECT_EQ(game->getGrid().getTile(7, 7).getGround() == TileGround::stone, true);
	EXPECT_EQ(game->getGrid().getTile(7, 7).getBlock() == TileBlock::wall, true);
	EXPECT_EQ(game->getGrid().getTile(7, 8).getBlock() == TileBlock::wall, false);

	auto it = game->getCharacters().begin();
	EXPECT_EQ(it->getPosition() == sf::Vector2u(7, 6), true);
	EXPECT_EQ(it->getTeam() == 1, true);
	it++;
	EXPECT_EQ(it->getPosition() == sf::Vector2u(7, 8), true);
	EXPECT_EQ(it->getTeam() == 2, true);

	EXPECT_EQ(game->getGrid().getTile(7, 8).getTopItem()->getType() == ItemType::Type_Weapon, true);
	EXPECT_EQ(game->getGrid().getTile(7, 8).getTopItem()->getIcon() == ItemIcon::Icon_Weapon_Shotgun, true);
}