#include "MainMenuScreen.hpp"
#include "Button.hpp"

MainMenuScreen::MainMenuScreen(void)
{
}

ScreenResult MainMenuScreen::Run(sf::RenderWindow & App)
{
	sf::Event Event;
	sf::Texture Texture;
	sf::Sprite Sprite;
	sf::Font Font;
	int menu = 0;
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
	 Button newgame("New game", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 250.f));
	 Button mapeditor("Map editor", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f));
	 Button quit("Quit", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 350.f));

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
		/*
		//Drawing the highlighted option
		if (menu == 0)
		{
			newgame.setState(hovered);
			mapeditor.setState(normal);
			quit.setState(normal);
		}
		else if (menu == 1)
		{
			newgame.setState(normal);
			mapeditor.setState(hovered);
			quit.setState(normal);
		} else {
			newgame.setState(normal);
			mapeditor.setState(normal);
			quit.setState(hovered);
		}*/

		// Sprite.setColor(sf::Color(255, 255, 255, 255));
		if(newgame.getState() == clicked) {return ScreenResult::NewGameScene;}
		if(mapeditor.getState() == clicked) {return ScreenResult::Exit;}
		if(quit.getState() == clicked) {return ScreenResult::Exit;}
		newgame.update(Event, App);
		mapeditor.update(Event, App);
		quit.update(Event, App);

		App.clear();
		App.draw(Sprite);
		App.draw(newgame.getText());
		App.draw(mapeditor.getText());
		App.draw(quit.getText());
		App.display();
	}

	return ScreenResult::Exit;
}
