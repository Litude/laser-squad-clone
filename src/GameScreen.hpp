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
	std::shared_ptr<sf::Font> font;
	std::shared_ptr<sf::Text> textCurTurn;
	std::shared_ptr<sf::View> gameView;
	std::shared_ptr<sf::View> fixedView;
	std::shared_ptr<sf::RectangleShape> interfaceBkg;
	std::shared_ptr<sf::RectangleShape> button;
	std::shared_ptr<sf::Text> textEndTurn;
	std::shared_ptr<sf::Text> textFPS;
	float lastTime = 0;
	float currentTime = 0;
	sf::Clock fpsclock;
	sf::Clock clock;
	double timeStep;
	double timeAccumulator;
	std::vector<sf::Sprite> characterShapes;
	std::vector<sf::Sprite> mapTiles;
	std::shared_ptr<sf::RectangleShape> selectedCharacter;
	std::shared_ptr<sf::Texture> texPlayer1;
	std::shared_ptr<sf::Texture> texPlayer2;
};

#endif