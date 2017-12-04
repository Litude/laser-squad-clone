#include "MainMenuScreen.hpp"
#include "Button.hpp"

MainMenuScreen::MainMenuScreen(void)
{
}

ScreenResult MainMenuScreen::Run(sf::RenderWindow & App)
{
	m_screenResult = ScreenResult::MainMenuScene;
	sf::Event Event;
	sf::Texture Texture;
	sf::Sprite Sprite;
	sf::Font Font;
	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::Red);
	rs.setSize(sf::Vector2f(20, 20));

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

	std::vector<Button> buttons;
	Button newgame("New game", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 250.f));
	newgame.setCallback([&] {this->openScreen(ScreenResult::NewGameScene); });
	buttons.push_back(newgame);

	Button mapeditor("Map editor", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f));
	mapeditor.setCallback([&] {this->openScreen(ScreenResult::Exit); });
	buttons.push_back(mapeditor);

	Button quit("Quit", Font, sf::Text::Bold, 25, sf::Vector2f(350.f, 350.f), rs);
	buttons.push_back(quit);
/*
	Button quit("Quit", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 350.f));
	quit.setCallback([&] {this->openScreen(ScreenResult::Exit); });
	buttons.push_back(quit);
*/
	auto selectedButtonItem = buttons.begin();

	while (m_screenResult == ScreenResult::MainMenuScene)
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
					if (selectedButtonItem > buttons.begin()) { selectedButtonItem--; }
					break;
				case sf::Keyboard::Down:
					if (selectedButtonItem < buttons.end() - 1) { selectedButtonItem++; }
					break;
				case sf::Keyboard::Return:
					selectedButtonItem->click();
					break;
				default:
					break;
				}
			}
		}

		for (auto it = buttons.begin(); it != buttons.end(); ++it) {
			it->setState(state::normal);
			it->update(Event, App);
			if(it->getState() == state::hovered){
				selectedButtonItem = it;
			}
		}

		selectedButtonItem->setState(state::hovered);
		App.clear();
		App.draw(Sprite);
		for (auto button : buttons) {
			App.draw(button);
		}
		App.display();
	}

	return m_screenResult;
}

void MainMenuScreen::openScreen(ScreenResult res)
{
	m_screenResult = res;
}
