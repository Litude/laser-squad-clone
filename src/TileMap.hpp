#include <SFML/Graphics.hpp>
#include "Map.hpp"

class TileMap : public sf::Drawable, public sf::Transformable
{
public:
	TileMap(Map &map);
	bool load(const std::string& tileset_ground, const std::string& tileset_block, sf::Vector2u tileSize);
	void updateTile(sf::Vector2u tilePosition);
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	void setGroundTile(Tile tile, sf::Vector2u tilePosition);
	void setBlockTile(Tile tile, sf::Vector2u tilePosition);
	Map &m_map;
	sf::Vector2u m_tileSize;
	sf::VertexArray m_vertices_ground;
	sf::VertexArray m_vertices_block;
	sf::Texture m_tileset_ground;
	sf::Texture m_tileset_block;
	enum WallParts { front, side, corner_bottom_left, corner_bottom_right, corner_top_left, corner_top_right, all_sides };
};