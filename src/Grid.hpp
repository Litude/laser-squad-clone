#ifndef GRID_H
#define GRID_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Tile.hpp"

class Grid {
public:
	typedef std::vector<Tile>::const_iterator const_iterator;
	typedef std::vector<Tile>::iterator iterator;

	Grid() { }

	Grid(unsigned size, unsigned width) : width(width), height(size/width), grid(size, Tile()) { }

	Grid(std::vector<Tile> grid, unsigned width) : width(width), grid(grid) { }

	unsigned getSize() const { return grid.size(); }
	unsigned getWidth() const { return width; }
	unsigned getHeight() const { return height; }

	const Tile operator()(int x, int y) const;
	const Tile operator()(sf::Vector2i loc) const;
	const Tile operator()(unsigned x, unsigned y) const;
	Tile& operator()(unsigned x, unsigned y);
	Tile& getTile(unsigned x, unsigned y);
	Tile& operator[](unsigned n);

	const_iterator begin() const;
	const_iterator end() const;
	iterator begin();
	iterator end();

	bool addTile( Tile t );

private:
	unsigned width;
	unsigned height;
	std::vector<Tile> grid;
};

std::ostream& operator<<(std::ostream& out, const Grid g);

#endif