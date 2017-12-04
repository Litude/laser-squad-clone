#ifndef LASER_GAMESCREEN_HPP
#define LASER_GAMESCREEN_HPP

#include <iostream>
#include <memory>
#include "Screen.hpp"
#include "Game.hpp"
#include "TileMap.hpp"
#include <SFML/Graphics.hpp>
#include "Util.hpp"

namespace MouseMode {
	enum Mode { shoot, select };
}

class GameScreen : public Screen
{
public:
	GameScreen(sf::RenderWindow &App);
	virtual ScreenResult Run(sf::RenderWindow &App);
private:
	void endTurn();
	void DrawGame(sf::RenderWindow &App);
	void DrawUI(sf::RenderWindow &App);
	sf::Vector2u getClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const;
	void DrawVisibleArea(sf::RenderWindow &App, std::vector<sf::Vector2u> visibleTiles);

	Game game;
	std::shared_ptr<TileMap> tileMap;
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Texture> texPlayer1;
	std::shared_ptr<sf::Texture> texPlayer2;
	std::shared_ptr<sf::Texture> texItems;
	sf::View gameView;
	sf::View interfaceView;
	sf::RectangleShape interfaceBkg;
	sf::RectangleShape buttonEndTurn{ sf::Vector2f(100, 80) };
	sf::RectangleShape buttonPickupItem{ sf::Vector2f(50, 20) };
	sf::RectangleShape buttonDropItem{ sf::Vector2f(50, 20) };
	sf::RectangleShape selectedCharacter;
	sf::RectangleShape selectedItem;
	sf::Text textCurTurn;
	sf::Text textEndTurn;
	sf::Text textFPS;
	sf::Text textAP;
	sf::Text textMouseMode;
	sf::Text textPickupItem;
	sf::Text textDropItem;
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

	MouseMode::Mode mouseMode = MouseMode::select;
	bool resized = true;
};

#endif