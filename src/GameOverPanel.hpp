#ifndef LASER_GAMEOVERPANEL_HPP
#define LASER_GAMEOVERPANEL_HPP

#include <iostream>
#include <memory>
#include <list>
#include "Game.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include "Util.hpp"

class GameScreen;

class GameOverPanel
{
public:
	GameOverPanel() {}
	GameOverPanel(sf::RenderWindow &App, GameScreen& gameScreen);
	void draw(sf::RenderWindow &App, Game &game, GameScreen& gameScreen);
	void update(sf::Event& event, sf::RenderWindow& App, Game &game);
	void updateLayout(sf::RenderWindow & App);
	void updateUIComponents(sf::RenderWindow & App);
private:
	std::shared_ptr<sf::Font> font;
	sf::View interfaceView;
	
	sf::RectangleShape interfaceBkg;
	Button buttonExit;
	sf::RectangleShape selectedItem;
	sf::RectangleShape equippedItem;
	sf::Text textGameOverLabel;
};

#endif