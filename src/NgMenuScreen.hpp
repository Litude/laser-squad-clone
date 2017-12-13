#ifndef LASER_NGMENUSCREEN_HPP
#define LASER_NGMENUSCREEN_HPP

#include <iostream>
#include <fstream>
#include "Screen.hpp"
#include "Button.hpp"
#include "TextField.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include "constants.hpp"

class NgMenuScreen : public Screen
{
public:
	NgMenuScreen(void);
	virtual ScreenResult Run(sf::RenderWindow &App);
	void openScreen(ScreenResult res);
	std::string getMapName();
private:
	ScreenResult m_screenResult;
private:
	// Components
	void drawUI(sf::RenderWindow &App);
	bool initComponents(sf::RenderWindow & App);
	void updateLayout(sf::RenderWindow & App);
	void checkMap(const std::string& mapname);

	std::shared_ptr<sf::Texture> backgroundTexture;
	sf::Sprite backgroundSprite;
	std::shared_ptr<sf::Texture> logoTexture;
	sf::Sprite logoSprite;
	std::shared_ptr<sf::Font> font;
	std::vector<Button> buttons;
	TextField tField;
	sf::Text screenStatusMessage;
	bool errorMessage;
	sf::Clock errorClock;
};

#endif
