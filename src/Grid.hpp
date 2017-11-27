#ifndef GRID_H
#define GRID_H

#include <vector>
#include <SFML/System/Vector2.hpp>
#include "Tile.hpp"

class Grid {
public:
	typedef std::vector<Tile>::const_iterator const_iterator;
	typedef std::vector<Tile>::iterator iterator;

	//Width must be manually set when using this constructor
	Grid() { }

	Grid(unsigned width) : width(width) { }

	Grid(unsigned size, unsigned width) : width(width), grid(size, Tile()) { }

	Grid(std::vector<Tile> grid, unsigned width) : width(width), grid(grid) { }

	unsigned getSize() const { return grid.size(); }
	unsigned getWidth() const { return width; }
	unsigned getHeight() const { return grid.size()/width; }

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
	void setWidth(unsigned n);

private:
	unsigned width;
	std::vector<Tile> grid;
};

std::ostream& operator<<(std::ostream& out, const Grid g);

#endif