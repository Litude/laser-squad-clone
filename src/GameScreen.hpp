#ifndef LASER_GAMESCREEN_HPP
#define LASER_GAMESCREEN_HPP

#include <iostream>
#include <memory>
#include "Screen.hpp"
#include "Game.hpp"
#include <SFML/Graphics.hpp>

class GameScreen : public Screen
{
public:
	GameScreen(sf::RenderWindow &App);
	virtual int Run(sf::RenderWindow &App);
private:
	void DrawGame(sf::RenderWindow &App);
	void DrawUI(sf::RenderWindow &App);
	sf::Vector2u GetClickedTilePosition(const sf::RenderWindow& App, const sf::Vector2i& point, const sf::View& view) const;

	Game game;
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Texture> texPlayer1;
	std::shared_ptr<sf::Texture> texPlayer2;
	sf::View gameView;
	sf::View interfaceView;
	sf::RectangleShape interfaceBkg;
	//sf::FloatRect buttonEndTurn { sf::Vector2f(100, 100), sf::Vector2f(100, 80)};
	sf::RectangleShape buttonEndTurn{ sf::Vector2f(100, 80) };
	sf::RectangleShape selectedCharacter;
	sf::Text textCurTurn;
	sf::Text textEndTurn;
	sf::Text textFPS;
	std::vector<sf::Sprite> mapTiles;
	sf::Clock fpsclock;
	sf::Clock clock;
	float lastTime = 0;
	float currentTime = 0;
	double timeStep = 16000;
	double timeAccumulator = 0;

	bool resized = true;
};

#endif