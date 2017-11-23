#ifndef LASER_MAINMENUSCREEN_HPP
#define LASER_MAINMENUSCREEN_HPP

#include <iostream>
#include "Screen.hpp"
#include <SFML/Graphics.hpp>

class MainMenuScreen : public Screen
{
public:
	MainMenuScreen(void);
	virtual int Run(sf::RenderWindow &App);
private:
	bool playing;
};

#endif
