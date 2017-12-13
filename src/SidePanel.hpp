#ifndef LASER_SIDEPANEL_HPP
#define LASER_SIDEPANEL_HPP

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

class GameScreen;

class SidePanel
{
public:
	SidePanel() {}
	SidePanel(sf::RenderWindow &App, GameScreen& gameScreen);
	void draw(sf::RenderWindow &App, Game &game, GameScreen& gameScreen);
	void update(sf::Event& event, sf::RenderWindow& App, Game &game);
	void updateLayout(sf::RenderWindow & App);
	void updateUIComponents(sf::RenderWindow & App);
private:
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Texture> texItems;
	sf::View interfaceView;
	
	sf::RectangleShape interfaceBkg;
	Button buttonEndTurn;
	Button buttonPickupItem;
	Button buttonDropItem;
	Button buttonEquipItem;
	Button buttonExit;
	Button buttonAttackMode;
	sf::RectangleShape selectedItem;
	sf::RectangleShape equippedItem;
	sf::Text textCurTurnLabel;
	sf::Text textCurTurnValue;
	sf::Text textTurnNoLabel;
	sf::Text textTurnNoValue;
	sf::Text textFPS;
	sf::Text textAPLabel;
	sf::Text textAPValue;
	sf::Text textHPLabel;
	sf::Text textHPValue;
	sf::Text textEquipped;

	sf::Clock fpsclock;
	sf::Clock clock;
	float lastTime = 0;
	float currentTime = 0;
	int timeStep = 16000;
	int timeAccumulator = 0;
	sf::Text selectedInventoryItemName;
	std::vector<sf::RectangleShape> inventoryItemBkg;
	std::vector<sf::Sprite> inventoryItems;
	std::vector<sf::Text> inventoryItemTexts;
	std::vector<Projectile> activeProjectiles;
	std::list<sf::Vector2u> visibleTiles;
};

#endif