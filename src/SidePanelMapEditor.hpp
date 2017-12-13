#ifndef LASER_SIDEPANELMAPEDITOR_HPP
#define LASER_SIDEPANELMAPEDITOR_HPP

#include <iostream>
#include <memory>
#include <list>
#include "Game.hpp"
#include "TileMap.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include "Util.hpp"
#include "Weapon.hpp"
#include "Projectile.hpp"
#include "TextField.hpp"

class MapEditor;

enum ElementType {
	Grounds,
	Blocks,
	Items,
	Characters,
};

class SidePanelMapEditor
{
public:
	SidePanelMapEditor() {}
	SidePanelMapEditor(sf::RenderWindow &App, MapEditor &editor);
	void draw(sf::RenderWindow &App);
	void update(sf::Event& event, sf::RenderWindow& App);
	void updateLayout(sf::RenderWindow & App);
	void updateUIComponents(sf::RenderWindow & App);
	const std::string getMapName() const;
	void setMapName(const std::string &name);
	void setActiveTileset(ElementType element);
private:
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Texture> texGrounds;
	std::shared_ptr<sf::Texture> texBlocks;
	std::shared_ptr<sf::Texture> texItems;
	std::shared_ptr<sf::Texture> texPlayer1;
	std::shared_ptr<sf::Texture> texPlayer2;
	std::shared_ptr<sf::Texture> texRemoveIcon;
	sf::View interfaceView;

	std::vector<Button> buttons_tilesets;
	std::vector<Button> buttons_grounds;
	std::vector<Button> buttons_blocks;
	std::vector<Button> buttons_items;
	std::vector<Button> buttons_characters;

	void saveMap(MapEditor &editor);

	Button createTilesetButton(ElementType element, MapEditor &editor);
	Button createGroundTileButton(TileGround tileGround, MapEditor &editor);
	Button createBlockTileButton(TileBlock tileBlock, MapEditor &editor);
	Button createRemoveBlockTileButton(MapEditor &editor);
	Button createItemButton(Item item, MapEditor &editor);
	Button createRemoveItemButton(MapEditor &editor);
	Button createCharacterButton(unsigned int team, MapEditor &editor);
	Button createRemoveCharacterButton(MapEditor &editor);
	
	Button buttonExit;
	Button buttonSaveMap;
	sf::RectangleShape selectedItem;
	sf::RectangleShape interfaceBkg;
	TextField mapNameField;
	sf::Text textTilesets;
	sf::Text textActiveElement;
	
	sf::RectangleShape selectedTileset;
	ElementType activeElements = Grounds;
};

#endif
