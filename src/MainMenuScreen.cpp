#include "MainMenuScreen.hpp"

MainMenuScreen::MainMenuScreen(void)
{
}

ScreenResult MainMenuScreen::Run(sf::RenderWindow & App)
{
	initComponents(App);
	m_screenResult = ScreenResult::MainMenuScene;
	sf::Event Event;

	auto selectedButtonItem = buttons.begin();

	while (m_screenResult == ScreenResult::MainMenuScene)
	{
		while (App.pollEvent(Event))
		{
			if (Event.type == sf::Event::Resized)
			{
				updateLayout(App);
			}

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
		drawUI(App);
	}

	return m_screenResult;
}

void MainMenuScreen::openScreen(ScreenResult res)
{
	m_screenResult = res;
}

void MainMenuScreen::drawUI(sf::RenderWindow &App)
{
	App.clear();
	App.draw(backgroundSprite);
	App.draw(logoSprite);
	for (auto button : buttons) {
		App.draw(button);
	}
	App.display();
}

void MainMenuScreen::updateLayout(sf::RenderWindow & App)
{
	App.setView(sf::View(sf::FloatRect(0, 0, App.getSize().x, App.getSize().y)));
	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);
	logoSprite.setPosition({ App.getView().getSize().x * 0.5f - logoSprite.getGlobalBounds().width * 0.5f, App.getView().getSize().y * 0.5f - logoSprite.getGlobalBounds().height * 1.f });
	unsigned int i = 0;
	for (auto &button : buttons) {
		button.setPosition({ App.getView().getSize().x * 0.5f, logoSprite.getPosition().y + logoSprite.getGlobalBounds().height * 1.1f + button.getGlobalBounds().height * 1.5f * i });
		i++;
	}
}

bool MainMenuScreen::initComponents(sf::RenderWindow & App)
{
	backgroundTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!backgroundTexture->loadFromFile("img/background.png"))
	{
		std::cerr << "Error loading background.png" << std::endl;
		return ScreenResult::Exit;
	}
	backgroundSprite.setTexture(*backgroundTexture);

	logoTexture = std::make_shared<sf::Texture>(sf::Texture());
	if (!logoTexture->loadFromFile("img/logo.png"))
	{
		std::cerr << "Error loading logo.png" << std::endl;
		return ScreenResult::Exit;
	}
	logoSprite.setTexture(*logoTexture);

	font = std::make_shared<sf::Font>(sf::Font());
	if (!font->loadFromFile("font/Pixellari.ttf"))
	{
		std::cerr << "Error loading Pixellari.ttf" << std::endl;
		return ScreenResult::Exit;
	}

	backgroundSprite.setScale(
		App.getView().getSize().x / backgroundSprite.getLocalBounds().width,
		App.getView().getSize().y / backgroundSprite.getLocalBounds().height);

	sf::RectangleShape rs;
	rs.setFillColor(sf::Color::White);
	rs.setSize(sf::Vector2f(170, 40));

	Button newgame("New game", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 250.f), rs);
	newgame.setCallback([&] {this->openScreen(ScreenResult::NewGameScene); });
	buttons.push_back(newgame);

	Button mapeditor("Map editor", *font, sf::Text::Regular, 25, sf::Vector2f(350.f, 300.f), rs);
	mapeditor.setCallback([&] {this->openScreen(ScreenResult::EditorScene); });
	buttons.push_back(mapeditor);

	Button quit("Quit", *font, sf::Text::Bold, 25, sf::Vector2f(350.f, 350.f), rs);
	quit.setCallback([&] {this->openScreen(ScreenResult::Exit); });
	buttons.push_back(quit);

	updateLayout(App);
}
