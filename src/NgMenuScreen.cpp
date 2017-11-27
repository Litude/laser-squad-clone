#include "NgMenuScreen.hpp"

NgMenuScreen::NgMenuScreen(void)
{
}

ScreenResult NgMenuScreen::Run(sf::RenderWindow & App) {
	sf::Event Event;
	sf::Texture Texture;
	sf::Sprite Sprite;
	sf::Font Font;
	sf::Text Menu1;
	sf::Text Menu2;
	sf::Text Menu3;
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
	Menu1.setFont(Font);
	Menu1.setCharacterSize(20);
	Menu1.setString("Launch game");
	Menu1.setPosition({ 350.f, 250.f });

	Menu2.setFont(Font);
	Menu2.setCharacterSize(20);
	Menu2.setString("Back");
	Menu2.setPosition({ 350.f, 300.f });

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
						return ScreenResult::MainMenuScene; // Back to main menu
					}
					break;
				default:
					break;
				}
			}
		}
		Sprite.setColor(sf::Color(255, 255, 255, 255));
		//Drawing the highlighted option
		if (menu == 0)
		{
			Menu1.setColor(sf::Color(0, 0, 0, highlighted_alpha));
			Menu1.setStyle(sf::Text::Bold);
			Menu2.setColor(sf::Color(0, 0, 0, non_highlighted_alpha));
			Menu2.setStyle(sf::Text::Regular);
			}
		else
		{
			Menu1.setColor(sf::Color(0, 0, 0, non_highlighted_alpha));
			Menu1.setStyle(sf::Text::Regular);
			Menu2.setColor(sf::Color(0, 0, 0, highlighted_alpha));
			Menu2.setStyle(sf::Text::Bold);
    }
		App.clear();

		App.draw(Sprite);
		App.draw(Menu1);
		App.draw(Menu2);
		App.display();
	}

	return ScreenResult::Exit;
}
