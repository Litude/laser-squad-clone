#ifndef LASER_SCREEN_HPP
#define LASER_SCREEN_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

enum ScreenResult {
	MainMenuScene, NewGameScene, GameScene, EditorScene, Exit
};

class Screen
{
public:
	Screen(void);
	virtual ScreenResult Run(sf::RenderWindow &App);
private:
};

#endif
