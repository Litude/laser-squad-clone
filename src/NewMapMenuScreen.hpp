#ifndef LASER_NEWMAPMENUSCREEN_HPP
#define LASER_NEWMAPMENUSCREEN_HPP

#include <iostream>
#include "Screen.hpp"
#include "Button.hpp"
#include "TextField.hpp"
#include <SFML/Graphics.hpp>
#include <memory>

enum MapInitType { new_map, load_map };

class NewMapMenuScreen : public Screen
{
public:
	NewMapMenuScreen(void);
	virtual ScreenResult Run(sf::RenderWindow &App);
	void openScreen(ScreenResult res);
	unsigned int getMapSizeX();
	unsigned int getMapSizeY();
	std::string getMapName();
	MapInitType getMapInitType() { return mapInitType; }
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
	sf::Sprite logoSprite;
	std::shared_ptr<sf::Font> font;
	std::vector<Button> buttons;

	TextField mapSizeXField;
	TextField mapSizeYField;
	TextField mapNameField;

	Button buttonCreateMap;
	Button buttonLoadMap;
	Button buttonBack;

	MapInitType mapInitType;
};

#endif
