#include <iostream>
#include "gtest/gtest.h"

#include "../Game.hpp"

TEST(GameLineOfSight, TwoCharactersCanSeeEachOtherWhenBlockIsRemovedBetweenThem) {
	int xSize = 15;
	int ySize = 15;

	Game game = Game();

	game.initializeGrid(xSize, ySize);
	// Add one solid block for collision testing
	game.getGrid().getTile(7, 7).setTile(TileGround::dirt, TileBlock::wall);

	// Add characters on opposite sides of the solid block
	// Team 1
	game.addCharacter(sf::Vector2u(7, 6), 1);
	// Team 2
	game.addCharacter(sf::Vector2u(7, 8), 2);

	// Characters should not see each other because of the block

	// Character 1
	game.setSelectedCharacter(game.getCharacters().begin());
	auto visibleTiles = game.seenCoordinates(game.getSelectedCharacter());
	bool found = std::find(visibleTiles.begin(), visibleTiles.end(), game.getCharacters()[1].getPosition()) != visibleTiles.end();
	EXPECT_EQ(found, false);
	// Character 2
	game.setSelectedCharacter(game.getCharacters().begin() + 1);
	visibleTiles = game.seenCoordinates(game.getSelectedCharacter());
	found = std::find(visibleTiles.begin(), visibleTiles.end(), game.getCharacters()[0].getPosition()) != visibleTiles.end();
	EXPECT_EQ(found, false);

	// Remove the block
	game.getGrid().getTile(7, 7).setTile(TileGround::dirt, TileBlock::air);

	// Characters should see each other since the block is gone

	// Character 1
	game.setSelectedCharacter(game.getCharacters().begin());
	visibleTiles = game.seenCoordinates(game.getSelectedCharacter());
	found = std::find(visibleTiles.begin(), visibleTiles.end(), game.getCharacters()[1].getPosition()) != visibleTiles.end();
	EXPECT_EQ(found, true);
	// Character 2
	game.setSelectedCharacter(game.getCharacters().begin() + 1);
	visibleTiles = game.seenCoordinates(game.getSelectedCharacter());
	found = std::find(visibleTiles.begin(), visibleTiles.end(), game.getCharacters()[0].getPosition()) != visibleTiles.end();
	EXPECT_EQ(found, true);
}