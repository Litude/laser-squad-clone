#include "config.h"
#include <iostream>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
#include "Game.hpp"
#include "Map.hpp"
#include "MainMenuScreen.hpp"
#include "GameScreen.hpp"
#include "NgMenuScreen.hpp"
#include "constants.hpp"

int main(int argc, char* argv[]) {

  sf::RenderWindow App(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Laser Squad");

  // Screens
  std::vector<Screen*> Screens;
  MainMenuScreen s0;
  Screens.push_back(&s0);
  GameScreen s1(App);
  Screens.push_back(&s1);
  NgMenuScreen s2;
  Screens.push_back(&s2);

  int screen = 0;

  //Main loop
  while (screen >= 0)
  {
	  screen = Screens[screen]->Run(App);
  }
}
