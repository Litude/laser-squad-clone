#ifndef LASER_NGMENUSCREEN_HPP
#define LASER_NGMENUSCREEN_HPP

#include <iostream>
#include "Screen.hpp"
#include <SFML/Graphics.hpp>

class NgMenuScreen : public Screen
{
public:
	NgMenuScreen(void);
	virtual int Run(sf::RenderWindow &App);
private:
};

#endif
