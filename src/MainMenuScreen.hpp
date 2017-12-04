#ifndef LASER_MAINMENUSCREEN_HPP
#define LASER_MAINMENUSCREEN_HPP

#include <iostream>
#include "Screen.hpp"
#include <SFML/Graphics.hpp>

class MainMenuScreen : public Screen
{
public:
	MainMenuScreen(void);
	virtual ScreenResult Run(sf::RenderWindow &App);
	void openScreen(ScreenResult res);
private:
	ScreenResult m_screenResult;
};

#endif
