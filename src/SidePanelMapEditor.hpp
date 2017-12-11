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

class MapEditor;

class SidePanelMapEditor
{
public:
	SidePanelMapEditor() {}
	SidePanelMapEditor(sf::RenderWindow &App, MapEditor &editor);
	void draw(sf::RenderWindow &App, Game &game, MapEditor& editor);
	void update(sf::Event& event, sf::RenderWindow& App, Game &game);
	void updateLayout(sf::RenderWindow & App);
	void updateUIComponents(sf::RenderWindow & App);
private:
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Texture> texGrounds;
	std::shared_ptr<sf::Texture> texBlocks;
	std::shared_ptr<sf::Texture> texItems;
	sf::View interfaceView;

	//std::vector<Button> buttons;
	std::vector<Button> buttons_grounds;
	std::vector<Button> buttons_blocks;

	Button createGroundTileButton(TileGround tileGround, MapEditor &editor);
	Button createBlockTileButton(TileBlock tileBlock, MapEditor &editor);
	
	sf::RectangleShape interfaceBkg;
	Button buttonExit;
	sf::RectangleShape selectedItem;
	sf::Text textFPS;

	sf::Clock fpsclock;
	sf::Clock clock;
	float lastTime = 0;
	float currentTime = 0;
	int timeStep = 16000;
	int timeAccumulator = 0;
};

#endif