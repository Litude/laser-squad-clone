#include "Grid.hpp"

/*const Tile Grid::operator()(int x, int y) const {
	if (x < 0 || y < 0) {
		return Tile(black, wall);
	} else {
		return (*this)((unsigned) x, (unsigned) y);
	}
}*/

const Tile Grid::operator()(sf::Vector2i loc) const {
	if (loc.x < 0 || loc.y < 0 || loc.x >= (int) width || loc.y >= (int) getHeight()) {
		return Tile(black, wall);
	} else {
		return (*this)(loc.x, loc.y);
	}
}

/*const Tile Grid::operator()(unsigned x, unsigned y) const {
	if (x >= width || y >= getHeight()) {
		return Tile(black, wall);
	} else {
		return grid[y*width + x];
	}
}*/

const Tile& Grid::operator()(sf::Vector2u loc) const {
	return (*this)(loc.x, loc.y);
}

Tile& Grid::operator()(sf::Vector2u loc) {
	return (*this)(loc.x, loc.y);
}

const Tile& Grid::operator()(int x, int y) const {
	if (x >= (int) width || y >= (int) getHeight()) {
		throw std::out_of_range("Tried to access Tile out of bounds at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	} else if (x < 0 || y < 0) {
		throw std::out_of_range("Tried to access Tile at negative coordinates (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	} else {
		return grid[y*width + x];
	}
}

Tile& Grid::operator()(int x, int y) {
	if (x >= (int) width || y >= (int) getHeight()) {
		throw std::out_of_range("Tried to access Tile out of bounds at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	} else if (x < 0 || y < 0) {
		throw std::out_of_range("Tried to access Tile at negative coordinates (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	} else {
		return grid[y*width + x];
	}
}

const Tile& Grid::operator()(unsigned x, unsigned y) const {
	if (x >= width || y >= getHeight()) {
		throw std::out_of_range("Tried to access Tile out of bounds at (" + std::to_string(x) + ", " + std::to_string(y) + ")");
	}
	return grid[y*width + x];

}

Tile& Grid::operator()(unsigned x, unsigned y) {
	return const_cast<Tile&>((*static_cast<const Grid*>(this))(x, y));
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

void Grid::setWidth(unsigned n) {
	width = n;
}

bool Grid::within_bounds(sf::Vector2u coords) {
	return (coords.x < getWidth() && coords.y < getHeight());
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
