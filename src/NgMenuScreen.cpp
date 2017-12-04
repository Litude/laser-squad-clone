#include "NgMenuScreen.hpp"
#include "Button.hpp"

NgMenuScreen::NgMenuScreen(void)
{
}

ScreenResult NgMenuScreen::Run(sf::RenderWindow & App) {
	m_screenResult = ScreenResult::NewGameScene;
	sf::Event Event;
	sf::Texture Texture;
	sf::Sprite Sprite;
	sf::Font Font;

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

	std::vector<Button> buttons;
	Button launchgame("Launch game", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 250.f));
	launchgame.setCallback([&] {this->openScreen(ScreenResult::GameScene); });
	buttons.push_back(launchgame);

	Button back("Back", Font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f));
	back.setCallback([&] {this->openScreen(ScreenResult::MainMenuScene); });
	buttons.push_back(back);

	auto selectedButtonItem = buttons.begin();

	while (m_screenResult == ScreenResult::NewGameScene)
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
					for (auto &button : buttons) {
						button.setState(state::normal);
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

void NgMenuScreen::openScreen(ScreenResult res)
{
	m_screenResult = res;
}
