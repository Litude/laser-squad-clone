#include "Tile.hpp"

TileBlock Tile::getBlock() const { return block; }

TileGround Tile::getGround() const { return ground; }

ItemSubType Tile::getTopItemType() const {
	if (items.size() == 0) return SubType_None;
	return items.at(items.size() - 1)->getSubType();
}

std::shared_ptr<Item> Tile::getTopItem() {
	if (items.size() == 0) return std::make_shared<Item>(Item());
	return items.at(items.size() - 1);
}

void Tile::popItem() {
	if (items.size() == 0) return;
	items.pop_back();
}

//const std::vector<Item>& Tile::getItems() const { return items; }

bool Tile::isSolid() const { return !(block == air); }

bool Tile::addItem(std::shared_ptr<Item> obj) {
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