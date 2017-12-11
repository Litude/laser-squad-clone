#ifndef LASER_SCREEN_HPP
#define LASER_SCREEN_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

enum ScreenResult {
	MainMenuScene, NewGameScene, GameScene, EditorScene, ControlsScene, NewMapMenuScene, Exit
};

class Screen
{
public:
	virtual ~Screen() = 0;
	virtual ScreenResult Run(sf::RenderWindow &App) = 0;
private:
};

#endif
