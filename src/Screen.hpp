#ifndef LASER_SCREEN_HPP
#define LASER_SCREEN_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

class Screen
{
public:
	Screen(void);
	virtual int Run(sf::RenderWindow &App);
private:
};

#endif