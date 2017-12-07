#ifndef LASER_GAMESCREEN_HPP
#define LASER_GAMESCREEN_HPP

#include <iostream>
#include <memory>
#include "Screen.hpp"
#include "Game.hpp"
#include "TileMap.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include "Util.hpp"
#include "Weapon.hpp"
#include "Projectile.hpp"

namespace MouseMode {
	enum Mode { shoot, select };
}

class GameScreen : public Screen
{
public:
	GameScreen(sf::RenderWindow &App);
	virtual ScreenResult Run(sf::RenderWindow &App);
private:
	ScreenResult m_screenResult;
private:
	void exitToMainMenu();
	void zoomViewAt(sf::Vector2i pixel, sf::RenderWindow& window, sf::View &view, float zoom);
	void endTurn();
	void pickupItem();
	void dropItem();
	void equipItem();
	void DrawGame(sf::RenderWindow &App);
	void DrawUI(sf::RenderWindow &App);
	void updateLayout(sf::RenderWindow & App);
	void updateUIComponents(sf::RenderWindow & App);
	sf::Vector2u getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const;
	void DrawVisibleArea(sf::RenderWindow &App, std::vector<sf::Vector2u> visibleTiles);
	void addProjectile(std::shared_ptr<Weapon> weapon, sf::Vector2u world_origin, sf::Vector2u world_destination);

	Game game;
	std::shared_ptr<TileMap> tileMap;
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Texture> texPlayer1;
	std::shared_ptr<sf::Texture> texPlayer2;
	std::shared_ptr<sf::Texture> texItems;
	std::shared_ptr<sf::Texture> backgroundTexture;
	sf::Sprite backgroundSprite;
	sf::View gameView;
	sf::View interfaceView;
	
	sf::RectangleShape interfaceBkg;
	Button buttonEndTurn;
	Button buttonPickupItem;
	Button buttonDropItem;
	Button buttonEquipItem;
	Button buttonExit;
	sf::RectangleShape selectedCharacter;
	sf::RectangleShape selectedItem;
	sf::RectangleShape equippedItem;
	sf::Text textCurTurnLabel;
	sf::Text textCurTurnValue;
	sf::Text textFPS;
	sf::Text textAPLabel;
	sf::Text textAPValue;
	sf::Text textMouseMode;
	sf::RectangleShape healthbarBkg;
	sf::RectangleShape healthbar;

	int rayIncr = 1;
	sf::VertexArray rayLine{ sf::Lines, 2 };

	std::vector<sf::Sprite> mapTiles;
	sf::Clock fpsclock;
	sf::Clock clock;
	float lastTime = 0;
	float currentTime = 0;
	double timeStep = 16000;
	double timeAccumulator = 0;
	std::shared_ptr<sf::RenderTexture> renderTexture_visibleTiles;
	sf::RectangleShape visibleTileShape;
	sf::Shader* shader;
	std::vector<sf::Sprite> inventoryItems;
	std::vector<sf::Text> inventoryItemTexts;
	std::vector<Projectile> activeProjectiles;

	MouseMode::Mode mouseMode = MouseMode::select;
};

#endif