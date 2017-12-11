#include "config.h"
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Game.hpp"
#include "Grid.hpp"
#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "NgMenuScreen.hpp"
#include <memory>
#include "constants.hpp"
#include "MapEditor.hpp"
#include "ControlsScreen.hpp"

int main(int argc, char* argv[]) {

	sf::RenderWindow App(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Laser Squad");

	ScreenResult result = ScreenResult::MainMenuScene;

	std::unique_ptr<Screen> screen;

	//Main loop
	while (result != ScreenResult::Exit)
	{
		switch (result)
		{
		case MainMenuScene:
			screen.reset(new MainMenuScreen());
			result = screen->Run(App);
			break;
		case NewGameScene:
			screen.reset(new NgMenuScreen());
			result = screen->Run(App);
			break;
		case GameScene:
			// TODO: Pass NewGameScreen parameters to GameScreen here
			screen.reset(new GameScreen(App));
			result = screen->Run(App);
			break;
		case EditorScene:
			// TODO: Pass NewGameScreen parameters to GameScreen here
			screen.reset(new MapEditor(App));
			result = screen->Run(App);
			break;
		case ControlsScene:
			screen.reset(new ControlsScreen());
			result = screen->Run(App);
			break;
		case Exit:
			exit(0);
			break;
		default:
			break;
		}
	}
}
