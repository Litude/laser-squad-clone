#ifndef TILE_MAP
#define TILE_MAP

#include <SFML/Graphics.hpp>
#include "Grid.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	TileMap(Grid &grid);
	bool load(const std::string& tileset_ground, const std::string& tileset_block, const std::string& tileset_items, sf::Vector2u tileSize);
	void updateTile(sf::Vector2u tilePosition);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setGroundTile(Tile tile, sf::Vector2u tilePosition);
	void setBlockTile(Tile tile, sf::Vector2u tilePosition);
	void setItemTile(Tile tile, sf::Vector2u tilePosition);
	Grid &m_grid;
	sf::Vector2u m_tileSize;
	sf::VertexArray m_vertices_ground;
	sf::VertexArray m_vertices_block;
	sf::VertexArray m_vertices_items;
	sf::Texture m_tileset_ground;
	sf::Texture m_tileset_block;
	sf::Texture m_tileset_items;
	enum WallParts { front, side, corner_bottom_left, corner_bottom_right, corner_top_left, corner_top_right, all_sides };
};

#endif
