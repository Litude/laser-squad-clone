#include "NgMenuScreen.hpp"
#include "Button.hpp"

NgMenuScreen::NgMenuScreen(void)
{
}

ScreenResult NgMenuScreen::Run(sf::RenderWindow & App) {
	sf::Event Event;
	sf::Texture Texture;
	sf::Sprite Sprite;
	sf::Font Font;
	int menu = 0;

	/*
		Variables to modify items on the main menu
	*/

	int highlighted_alpha = 255; // Opacity of the currently chosen option
	int non_highlighted_alpha = 100; // Opacity of the other options on the menu
	int nofMenus = 2; // Number of items currently in the main menu

	if (!Texture.loadFromFile("img/newgamemenu.png"))
	{
		std::cerr << "Error loading newgamemenu.png" << std::endl;
		return ScreenResult::Exit;
	}
	Sprite.setTexture(Texture);

	if (!Font.loadFromFile("font/ARIALN.TTF"))
	{
		std::cerr << "Error loading ARIALN.TTF" << std::endl;
		return ScreenResult::Exit;
	}

	Button launchgame("Launch game", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 250.f));
	Button back("Back", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f));

	while (1)
	{
		while (App.pollEvent(Event))
		{
			if (Event.type == sf::Event::Closed)
			{
				return ScreenResult::Exit;
			}
			if (Event.type == sf::Event::KeyPressed)
			{
				switch (Event.key.code)
				{
				case sf::Keyboard::Up:
				  if(menu > 0) {menu--;}
					break;
				case sf::Keyboard::Down:
					if(menu < nofMenus - 1) {menu++;}
					break;
				case sf::Keyboard::Return:
					if (menu == 0)
					{
						return ScreenResult::GameScene; //New game
					} else {
						std::cout << " " + back.getState();
						return ScreenResult::MainMenuScene; // Back to main menu
					}
					break;
				default:
					break;
				}
			}
		}
		if(launchgame.getState() == clicked) {return ScreenResult::GameScene;}
		if(back.getState() == clicked) {return ScreenResult::MainMenuScene;}

		launchgame.update(Event, App);
		back.update(Event, App);
		App.clear();

		App.draw(Sprite);
		App.draw(launchgame.getText());
		App.draw(back.getText());
		App.display();
	}

	return ScreenResult::Exit;
}
