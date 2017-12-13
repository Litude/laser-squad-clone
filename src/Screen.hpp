#ifndef LASER_SCREEN_HPP
#define LASER_SCREEN_HPP

#include <iostream>
#include <SFML/Graphics.hpp>

enum ScreenResult {
	MainMenuScene, NewGameScene, GameScene, EditorScene, ControlsScene, NewMapMenuScene, EndTurnScene, Exit
};

class Screen
{
public:
	virtual ~Screen() {}

	Screen() = default;
	Screen(const Screen& copyFrom) = default;
	Screen& operator=(const Screen& copyFrom) = default;
	Screen(Screen &&) = default;
	Screen& operator=(Screen &&) = default;
	virtual ScreenResult Run(sf::RenderWindow &App) = 0;
private:
};

#endif
