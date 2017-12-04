#ifndef LASER_NGMENUSCREEN_HPP
#define LASER_NGMENUSCREEN_HPP

#include <iostream>
#include "Screen.hpp"
#include <SFML/Graphics.hpp>

class NgMenuScreen : public Screen
{
public:
	NgMenuScreen(void);
	virtual ScreenResult Run(sf::RenderWindow &App);
	void openScreen(ScreenResult res);
private:
	ScreenResult m_screenResult;
};

#endif
