#include "MainMenuScreen.hpp"

MainMenuScreen::MainMenuScreen(void)
{
}

ScreenResult MainMenuScreen::Run(sf::RenderWindow & App)
{
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
	int nofMenus = 3; // Number of items currently in the main menu

	if (!Texture.loadFromFile("img/mainmenu.png"))
	{
		std::cerr << "Error loading mainmenu.png" << std::endl;
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
	Menu1.setString("New game");
	Menu1.setPosition({ 350.f, 250.f });

	Menu2.setFont(Font);
	Menu2.setCharacterSize(20);
	Menu2.setString("Map editor");
	Menu2.setPosition({ 350.f, 300.f });

	Menu3.setFont(Font);
	Menu3.setCharacterSize(20);
	Menu3.setString("Exit");
	Menu3.setPosition({ 350.f, 350.f });

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
						return ScreenResult::NewGameScene; //New game
					} else if(menu == 1) {
						return ScreenResult::Exit; // TO DO: Map editor screen
					}
					else if(menu == 2)
					{
						return ScreenResult::Exit; //Exit game
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
			Menu3.setColor(sf::Color(0, 0, 0, non_highlighted_alpha));
			Menu3.setStyle(sf::Text::Regular);
		}
		else if (menu == 1)
		{
			Menu1.setColor(sf::Color(0, 0, 0, non_highlighted_alpha));
			Menu1.setStyle(sf::Text::Regular);
			Menu2.setColor(sf::Color(0, 0, 0, highlighted_alpha));
			Menu2.setStyle(sf::Text::Bold);
			Menu3.setColor(sf::Color(0, 0, 0, non_highlighted_alpha));
			Menu3.setStyle(sf::Text::Regular);
		} else {
			Menu1.setColor(sf::Color(0, 0, 0, non_highlighted_alpha));
			Menu1.setStyle(sf::Text::Regular);
			Menu2.setColor(sf::Color(0, 0, 0, non_highlighted_alpha));
			Menu2.setStyle(sf::Text::Regular);
			Menu3.setColor(sf::Color(0, 0, 0, highlighted_alpha));
			Menu3.setStyle(sf::Text::Bold);
		}
		App.clear();

		App.draw(Sprite);
		App.draw(Menu1);
		App.draw(Menu2);
		App.draw(Menu3);
		App.display();
	}

	return ScreenResult::Exit;
}
