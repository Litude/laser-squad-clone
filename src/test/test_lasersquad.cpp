#include <iostream>
#include "gtest/gtest.h"

#include "../Grid.hpp"

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