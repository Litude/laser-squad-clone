#include "Tile.hpp"

TileBlock Tile::getBlock() const { return block; }

TileGround Tile::getGround() const { return ground; }

sf::Texture& Tile::getTexture() { return texture; }

const std::vector<int>& Tile::getItems() const { return items; }

bool Tile::isSolid() const { return !(block == air); }

std::ostream& operator<<(std::ostream& os, const Tile& t) {

	switch (t.getBlock()) {
		case wall:
			os << " w ";
			break;
		case tree:
			os << " t ";
			break;
		case bush:
			os << " b ";
			break;
		default:
			os << " _ ";
			break;
	}

	return os;
}

void Tile::setTile(TileGround tg, TileBlock tb) {
	ground = tg;
	block = tb;
}