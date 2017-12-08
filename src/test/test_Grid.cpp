#include <iostream>
#include "gtest/gtest.h"

#include "../Grid.hpp"
#include "../Health.hpp"

TEST(GridInitSizes, HasCorrectDimensionsAndSizes) {
	int xSize = 15;
	int ySize = 15;
	Grid grid = Grid(xSize*ySize, xSize);
	EXPECT_EQ(grid.getWidth(), xSize);
	EXPECT_EQ(grid.getHeight(), ySize);
	EXPECT_EQ(grid.getSize(), xSize*ySize);
}

TEST(GridInitSizes, HasCorrectDimensionsAndSizes2) {
	int xSize = 15;
	int ySize = 25;
	Grid grid = Grid(xSize*ySize, xSize);
	EXPECT_EQ(grid.getWidth(), xSize);
	EXPECT_EQ(grid.getHeight(), ySize);
	EXPECT_EQ(grid.getSize(), xSize*ySize);
}

TEST(GridInitSizes, HasCorrectDimensionsAndSizes3) {
	int xSize = 1;
	int ySize = 1;
	Grid grid = Grid(xSize*ySize, xSize);
	EXPECT_EQ(grid.getWidth(), xSize);
	EXPECT_EQ(grid.getHeight(), ySize);
	EXPECT_EQ(grid.getSize(), xSize*ySize);
}

TEST(GridChangeTiles, HasChangedTilesCorrectly) {
	int xSize = 15;
	int ySize = 15;
	Grid grid = Grid(xSize*ySize, xSize);
	grid.getTile(0, 0).setTile(TileGround::dirt, TileBlock::wall);
	grid.getTile(14, 14).setTile(TileGround::dirt, TileBlock::wall);
	EXPECT_EQ(grid.getTile(0, 0).getBlock(), TileBlock::wall);
	EXPECT_EQ(grid.getTile(14, 14).getBlock(), TileBlock::wall);
	EXPECT_EQ(grid.getTile(0, 1).getBlock(), TileBlock::air);
	EXPECT_EQ(grid.getTile(1, 1).getBlock(), TileBlock::air);
	grid.getTile(14, 14).setTile(TileGround::dirt, TileBlock::air);
	EXPECT_EQ(grid.getTile(14, 14).getBlock(), TileBlock::air);
}

TEST(GridChangeTiles, HasChangedTileItemsCorrectly) {
	int xSize = 15;
	int ySize = 15;
	Grid grid = Grid(xSize*ySize, xSize);
	auto item1 = std::make_shared<HealthPackLarge>(HealthPackLarge());
	auto item2 = std::make_shared<HealthPackLarge>(HealthPackLarge());
	auto item3 = std::make_shared<HealthPackLarge>(HealthPackLarge());
	grid.getTile(0, 0).addItem(item1);
	grid.getTile(0, 0).addItem(item2);
	grid.getTile(0, 0).addItem(item3);
	EXPECT_EQ(grid.getTile(0, 0).getTopItem(), item3);
	grid.getTile(0, 0).popItem();
	EXPECT_EQ(grid.getTile(0, 0).getTopItem(), item2);
	grid.getTile(0, 0).popItem();
	EXPECT_EQ(grid.getTile(0, 0).getTopItem(), item1);
	grid.getTile(0, 0).popItem();
	EXPECT_EQ(grid.getTile(0, 0).getTopItem()->getType(), ItemType::Type_None);
	grid.getTile(0, 0).popItem();
	EXPECT_EQ(grid.getTile(0, 0).getTopItem()->getType(), ItemType::Type_None);
}