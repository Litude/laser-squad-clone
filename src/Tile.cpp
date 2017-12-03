#include "Tile.hpp"

TileBlock Tile::getBlock() const { return block; }

TileGround Tile::getGround() const { return ground; }

ItemSubType Tile::getTopItemType() const {
	if (items.size() == 0) return SubType_None;
	return items.at(items.size() - 1).getSubType();
}

sf::Texture& Tile::getTexture() { return texture; }

const std::vector<Item>& Tile::getItems() const { return items; }

bool Tile::isSolid() const { return !(block == air); }

bool Tile::addItem(Item obj) {
	items.push_back(obj);
	return true;
}

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