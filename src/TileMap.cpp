#include "TileMap.hpp"

TileMap::TileMap(Grid &grid) :
	m_grid(grid)
{
}

bool TileMap::load(const std::string& tileset_ground, const std::string& tileset_block, sf::Vector2u tileSize)
{
	m_tileSize = tileSize;

	// Load the tileset texture for grounds
	if (!m_tileset_ground.loadFromFile(tileset_ground))
		return false;

	// Load the tileset texture for blocks
	if (!m_tileset_block.loadFromFile(tileset_block))
		return false;

	// Resize the ground and block vertex arrays to fit the grid size
	m_vertices_ground.setPrimitiveType(sf::Quads);
	m_vertices_ground.resize(m_grid.getSize() * 4);
	m_vertices_block.setPrimitiveType(sf::Quads);
	m_vertices_block.resize(m_grid.getSize() * 4);

	// Populate the ground and block vertex arrays, with one quad per tile
	for (unsigned int i = 0; i < m_grid.getWidth(); ++i)
	{
		for (unsigned int j = 0; j < m_grid.getHeight(); ++j)
		{
			Tile tile = m_grid.getTile(i, j);
			setGroundTile(tile, sf::Vector2u(i, j));
			setBlockTile(tile, sf::Vector2u(i, j));
		}
	}
	return true;
}

void TileMap::updateTile(sf::Vector2u tilePosition)
{
	Tile tile = m_grid.getTile(tilePosition.x, tilePosition.y);
	// Update both vertex arrays
	setGroundTile(tile, tilePosition);
	setBlockTile(tile, tilePosition);
}

void TileMap::setGroundTile(Tile tile, sf::Vector2u tilePosition)
{
	int i = tilePosition.x;
	int j = tilePosition.y;

	// Determine the tile to draw on the tileset
	int tileNumber = tile.getGround();

	// Find tile's position in the ground tileset texture
	int tu = tileNumber % (m_tileset_ground.getSize().x / m_tileSize.x);
	int tv = tileNumber / (m_tileset_ground.getSize().x / m_tileSize.x);

	// Get a pointer to the current tile's quad
	sf::Vertex* quad = &m_vertices_ground[(i + j * m_grid.getWidth()) * 4];

	// Define tile's 4 corners
	quad[0].position = sf::Vector2f(i * m_tileSize.x, j * m_tileSize.y);
	quad[1].position = sf::Vector2f((i + 1) * m_tileSize.x, j * m_tileSize.y);
	quad[2].position = sf::Vector2f((i + 1) * m_tileSize.x, (j + 1) * m_tileSize.y);
	quad[3].position = sf::Vector2f(i * m_tileSize.x, (j + 1) * m_tileSize.y);

	// Define tile's 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
	quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
	quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
	quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);
}

void TileMap::setBlockTile(Tile tile, sf::Vector2u tilePosition)
{
	int i = tilePosition.x;
	int j = tilePosition.y;

	// Determine the tile to draw on the tileset
	int tileNumber_x = tile.getBlock();
	int tileNumber_y = 0;

	// Special case for wall blocks
	if (tile.getBlock() == TileBlock::wall) {
		bool up = j > 0 ? m_grid.getTile(i, j - 1).getBlock() == TileBlock::wall : false;
		bool down = j < m_grid.getHeight() - 1 ? m_grid.getTile(i, j + 1).getBlock() == TileBlock::wall : false;
		bool left = i > 0 ? m_grid.getTile(i - 1, j).getBlock() == TileBlock::wall : false;
		bool right = i < m_grid.getWidth() - 1 ? m_grid.getTile(i + 1, j).getBlock() == TileBlock::wall: false;

		if (up && down && left && right) {
			tileNumber_y = WallParts::all_sides;
		}
		else if ((up && down && left) || (up && down && right) || (up && left && right) || (down && left && right)) {
			tileNumber_y = WallParts::all_sides;
		}
		else if (up && down) {
			tileNumber_y = WallParts::side;
		}
		else if (up && left) {
			tileNumber_y = WallParts::corner_bottom_left;
		}
		else if (up && right) {
			tileNumber_y = WallParts::corner_bottom_right;
		}
		else if (down && left) {
			tileNumber_y = WallParts::corner_top_left;
		}
		else if (down && right) {
			tileNumber_y = WallParts::corner_top_right;
		}
		else if (left && right) {
			tileNumber_y = WallParts::front;
		}
		else if (up || down) {
			tileNumber_y = WallParts::side;
		}
		else if (left || right) {
			tileNumber_y = WallParts::front;
		}
	}

	// Find tile's position in the block tileset texture
	int tu = tileNumber_x % (m_tileset_block.getSize().x / m_tileSize.x);
	int tv = tileNumber_y % (m_tileset_block.getSize().y / m_tileSize.y);

	// Get a pointer to the current tile's quad
	sf::Vertex* quad = &m_vertices_block[(i + j * m_grid.getWidth()) * 4];

	// Define tile's 4 corners
	quad[0].position = sf::Vector2f(i * m_tileSize.x, j * m_tileSize.y);
	quad[1].position = sf::Vector2f((i + 1) * m_tileSize.x, j * m_tileSize.y);
	quad[2].position = sf::Vector2f((i + 1) * m_tileSize.x, (j + 1) * m_tileSize.y);
	quad[3].position = sf::Vector2f(i * m_tileSize.x, (j + 1) * m_tileSize.y);

	// Define tile's 4 texture coordinates
	quad[0].texCoords = sf::Vector2f(tu * m_tileSize.x, tv * m_tileSize.y);
	quad[1].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, tv * m_tileSize.y);
	quad[2].texCoords = sf::Vector2f((tu + 1) * m_tileSize.x, (tv + 1) * m_tileSize.y);
	quad[3].texCoords = sf::Vector2f(tu * m_tileSize.x, (tv + 1) * m_tileSize.y);
}

void TileMap::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	// Apply the transform
	states.transform *= getTransform();

	// Apply the tileset texture for ground tiles
	states.texture = &m_tileset_ground;

	// Draw the vertex array for ground tiles
	target.draw(m_vertices_ground, states);

	// Apply the tileset texture for block tiles
	states.texture = &m_tileset_block;

	// Draw the vertex array for block tiles
	target.draw(m_vertices_block, states);
}