#include "MainMenuScreen.hpp"

MainMenuScreen::MainMenuScreen(void)
{
	bool playing = false;
}

int MainMenuScreen::Run(sf::RenderWindow & App)
{
	std::cout << "mainmenuscreen test";
	sf::Event Event;
	bool Running = true;
	sf::Texture Texture;
	sf::Sprite Sprite;
	int alpha = 0;
	sf::Font Font;
	sf::Text Menu1;
	sf::Text Menu2;
	sf::Text Menu3;
	int menu = 0;

	if (!Texture.loadFromFile("img/mainmenu.png"))
	{
		std::cerr << "Error loading mainmenu.png" << std::endl;
		return (-1);
	}
	Sprite.setTexture(Texture);
	Sprite.setColor(sf::Color(255, 255, 255, alpha));
	if (!Font.loadFromFile("font/ARIALN.TTF"))
	{
		std::cerr << "Error loading ARIALN.TTF" << std::endl;
		return (-1);
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

	while (Running)
	{
		//Verifying events
		while (App.pollEvent(Event))
		{
			// Window closed
			if (Event.type == sf::Event::Closed)
			{
				return (-1);
			}
			//Key pressed
			if (Event.type == sf::Event::KeyPressed)
			{
				switch (Event.key.code)
				{
				case sf::Keyboard::Up:
				  std::cout << "up";
					menu = 0;
					break;
				case sf::Keyboard::Down:
					std::cout << "down";
					menu = 1;
					break;
				case sf::Keyboard::Return:
					if (menu == 0)
					{
						//Let's get play !
						std::cout << "play";
						playing = true;
						return (1);
					}
					else
					{
						//Let's get work...
						return (-1);
					}
					break;
				default:
					break;
				}
			}
		}
		Sprite.setColor(sf::Color(255, 255, 255, 255));
		if (menu == 0)
		{
			Menu1.setColor(sf::Color(0, 0, 0, 255));
			Menu1.setStyle(sf::Text::Bold);
			Menu2.setColor(sf::Color(0, 0, 0, 100));
			Menu2.setStyle(sf::Text::Regular);
			Menu3.setColor(sf::Color(0, 0, 0, 100));
			Menu3.setStyle(sf::Text::Regular);
		}
		else if (menu == 1)
		{
			Menu1.setColor(sf::Color(0, 0, 0, 100));
			Menu1.setStyle(sf::Text::Regular);
			Menu2.setColor(sf::Color(0, 0, 0, 255));
			Menu2.setStyle(sf::Text::Bold);
			Menu3.setColor(sf::Color(0, 0, 0, 100));
			Menu3.setStyle(sf::Text::Regular);
		} else {
			Menu1.setColor(sf::Color(0, 0, 0, 100));
			Menu1.setStyle(sf::Text::Regular);
			Menu2.setColor(sf::Color(0, 0, 0, 100));
			Menu2.setStyle(sf::Text::Regular);
			Menu3.setColor(sf::Color(0, 0, 0, 255));
			Menu3.setStyle(sf::Text::Bold);
		}
		//Clearing screen
		App.clear();
		//Drawing
		App.draw(Sprite);
		App.draw(Menu1);
		App.draw(Menu2);
		App.draw(Menu3);
		App.display();
	}

	//Never reaching this point normally, but just in case, exit the application
	return (-1);
}
