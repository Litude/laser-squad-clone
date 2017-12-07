#include "Screen.hpp"

Screen::Screen(void)
{
}

Screen::~Screen()
{
}

ScreenResult Screen::Run(sf::RenderWindow & App)
{
	return ScreenResult::Exit;
}
