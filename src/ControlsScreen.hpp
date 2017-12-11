#ifndef LASER_CONTROLSSCREEN_HPP
#define LASER_CONTROLSSCREEN_HPP

#include <iostream>
#include "Screen.hpp"
#include "Button.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

class ControlsScreen : public Screen
{
public:
	ControlsScreen(void);
	virtual ScreenResult Run(sf::RenderWindow &App);
	void openScreen(ScreenResult res);
private:
	ScreenResult m_screenResult;
private:
	// Components
	void drawUI(sf::RenderWindow &App);
	bool initComponents(sf::RenderWindow & App);
	void updateLayout(sf::RenderWindow & App);

	std::shared_ptr<sf::Texture> backgroundTexture;
	sf::Sprite backgroundSprite;
	std::shared_ptr<sf::Texture> logoTexture;
  sf::RectangleShape rec;
	sf::Sprite logoSprite;
	std::shared_ptr<sf::Font> font;
	std::vector<Button> buttons;
};

#endif
