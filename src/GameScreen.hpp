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
	Game game;
	void DrawUI(sf::RenderWindow &App);
	std::shared_ptr<sf::Font> font;
	sf::Text textCurTurn;
	sf::View gameView;
	sf::View fixedView;
	sf::RectangleShape interfaceBkg;
	sf::RectangleShape button;
	sf::Text textEndTurn;
	sf::Text textFPS;
	float lastTime = 0;
	float currentTime = 0;
	sf::Clock fpsclock;
	sf::Clock clock;
	double timeStep;
	double timeAccumulator;
	sf::Sprite characterShapes;
	std::vector<sf::Sprite> mapTiles;
	sf::RectangleShape selectedCharacter;
	std::shared_ptr<sf::Texture> texPlayer1;
	std::shared_ptr<sf::Texture> texPlayer2;
};

#endif