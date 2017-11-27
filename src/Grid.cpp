#include "Grid.hpp"

const Tile& Grid::operator()(unsigned x, unsigned y) const {
	return grid[y*width + x];
}

Tile& Grid::operator()(unsigned x, unsigned y) {
	return grid[y*width + x];
}

Tile& Grid::getTile(unsigned x, unsigned y) {
	return (*this)(x, y);
}

Tile& Grid::operator[](unsigned n) {
	return grid[n];
}

bool Grid::addTile(Tile t) {
	grid.push_back(t);
	return true;
}

Grid::const_iterator Grid::begin() const {
	return grid.begin();
}

Grid::const_iterator Grid::end() const {
	return grid.end();
}

Grid::iterator Grid::begin() {
	return grid.begin();
}

Grid::iterator Grid::end() {
	return grid.end();
}

std::ostream& operator<<(std::ostream& out, const Grid g) {
	unsigned k = 0;
	for (auto it = g.begin(); it != g.end(); ++it) {
		out << *it;
		++k;
		if (k == g.getWidth()) {
			out << std::endl;
			k = 0;
		}
	}
	return out;
}
